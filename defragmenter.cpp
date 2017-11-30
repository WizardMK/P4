#include "QuadraticProbing.h"
#include "defragmenter.h"
#include "mynew.h"
#include "DefragRunner.h"
#include "StackAr.h"

Defragmenter::Defragmenter(DiskDrive *diskDrive) { 
  HashBlock zero; // standard, unhashed, unused block
  QuadraticHashTable<HashBlock> hashTable(zero, 500000);//HashTable for keeping track of the positions of moved diskblocks
  int index = 2; // blocks begin at 2
  StackAr<int> emptyStack (390000); // keep track of empties

  for(int i = 2; i < 500000; i++) {
    if(diskDrive->FAT[i] == false)
      emptyStack.push(i); 
  }

  for(int i = 0; i < diskDrive->getNumFiles(); i++) { // for each file
    int blockID = diskDrive->directory[i].getFirstBlockID();
    diskDrive->directory[i].setFirstBlockID(index);

    for (unsigned j = 0; j < diskDrive->directory[i].getSize(); j++, index++) { // for size of file
      DiskBlock* nextBlock;
      DiskBlock* indexBlock;

      HashBlock result = hashTable.findInt(blockID); // look for a hash

      int hashedLoc = 0;
      bool hashedBlock = false;

      while(zero.oldLoc != result.oldLoc) {
        hashedBlock = true;
        hashedLoc = result.newLoc;
        hashTable.remove(result);
        result = hashTable.findInt(result.newLoc); // until not found
      }

      if (hashedBlock == true) {
        emptyStack.push(hashedLoc); 
        diskDrive->FAT[hashedLoc] = false;
        hashTable.remove(result); // delete hash, since we need hashed block moved to index

        if(diskDrive->FAT[index] == false) {
          nextBlock = diskDrive->readDiskBlock(hashedLoc);
          diskDrive->writeDiskBlock(nextBlock,index); // write the hashed block to index
          diskDrive->FAT[index] = true;
          blockID = nextBlock->getNext();
          delete nextBlock;
        }

        else {
          nextBlock = diskDrive->readDiskBlock(hashedLoc);
          indexBlock = diskDrive->readDiskBlock(index);

          int maxEmpty = emptyStack.topAndPop();

          diskDrive->writeDiskBlock(nextBlock, index);
          diskDrive->writeDiskBlock(indexBlock, maxEmpty);
          diskDrive->FAT[index] = true;
          diskDrive->FAT[maxEmpty] = true;
          result.oldLoc = index;
          result.newLoc = maxEmpty;
          hashTable.insert(result);

          blockID = nextBlock->getNext();
          delete nextBlock;
          delete indexBlock;
        }

      } // if

      else {
        if (emptyStack.top() - 40000 <= blockID ) 
          emptyStack.push(blockID);
        diskDrive->FAT[blockID] = false;
        
        if(diskDrive->FAT[index] == false) {
          nextBlock = diskDrive->readDiskBlock(blockID);
          diskDrive->writeDiskBlock(nextBlock,index);
          diskDrive->FAT[index] = true;
          blockID = nextBlock->getNext();
          delete nextBlock; 
        }

        else {
          HashBlock newHash;
          nextBlock = diskDrive->readDiskBlock(blockID); // reads block that needs to be moved to index
          indexBlock = diskDrive->readDiskBlock(index); // stores block at current index

          int maxEmpty = emptyStack.topAndPop();

          diskDrive->writeDiskBlock(nextBlock, index); 
          diskDrive->writeDiskBlock(indexBlock, maxEmpty); // move it to biggest empty
          diskDrive->FAT[index] = true;
          diskDrive->FAT[maxEmpty] = true;
          newHash.oldLoc = index;
          newHash.newLoc = maxEmpty;
          hashTable.insert(newHash);
                
          blockID = nextBlock->getNext();
          delete nextBlock;
          delete indexBlock;
        } // else
      } // else
    } // for
  } // for
} // defrag