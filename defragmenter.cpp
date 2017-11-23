
// Version 1
#include "defragmenter.h"
#include "DefragRunner.h"
#include "mynew.h"
#include "QuadraticProbing.h"
#include "StackAr.h"
#include <iostream>

using namespace std;

Defragmenter::Defragmenter(DiskDrive* diskDrive)
{
	StackAr<int> emptyStack (50000);
	QuadraticHashTable<int>* HT;
	HT = new QuadraticHashTable<int> (-1, 200000);
	DiskBlock* arr[15000];
	int fileNum = diskDrive->getNumFiles();
	int cap = diskDrive->getCapacity();
	int currentBlockID = diskDrive->directory[0].getFirstBlockID();
	int sizeDir = 2; // file size counter, used for setting 1st block ID in directory
	int writeBlockIDCounter = 2; // file block number counter
	int dirCounter = 0; // directory counter
	int newPosHT = 0;
	DiskBlock* dBlockPTR = NULL;
	DiskBlock* dBlockPTR2 = NULL;

	for (int i = 2; i < cap; i++) {
		if (diskDrive->FAT[i] == false) {  // Build stack of empties
			emptyStack.push(i);
	    }
	}

	for (int i = 2; i < cap; i++) { // Store as much of the DBlocks in RAM as possible
		dBlockPTR = diskDrive->readDiskBlock(i); 
		if ( i < ((int) sizeof(arr))/sizeof(arr[0])) { // read into array, maximize RAM
			arr[i] = dBlockPTR;
		}

	/*	if (i >= ((int) sizeof(arr))/sizeof(arr[0])) { // read into hash 
			HT->insert(i, emptyStack.top()); // hash the outlying blockPTR to the biggest empty index, near the bottom
			diskDrive->writeDiskBlock(dBlockPTR, emptyStack.top());
			diskDrive->FAT[emptyStack.topAndPop()] = true;
		}*/
	}


	for (int i = 0; i < fileNum; i++) { // for each file





		do {

			if (currentBlockID > ((int) sizeof(arr))/sizeof(arr[0])) {
				cout << "Greater than maxSize" << endl;
				newPosHT = HT->find(currentBlockID);
				dBlockPTR2 = diskDrive->readDiskBlock(newPosHT);
				diskDrive->writeDiskBlock(dBlockPTR2 , newPosHT); // move dBlock to furthest down empty spot
				diskDrive->FAT[i] = true;
				writeBlockIDCounter++;
				currentBlockID = dBlockPTR2->getNext();


			}
			else { 
				diskDrive->writeDiskBlock(arr[currentBlockID], writeBlockIDCounter);
				//arr[currentBlockID]->setNext(writeBlockIDCounter );
				diskDrive->FAT[i] = true;
			  //cout << "writeBlockIDCounter: " << writeBlockIDCounter << " ";
				//cout << "currentBlockID: " << currentBlockID << endl;
				writeBlockIDCounter++;
				currentBlockID = arr[currentBlockID]->getNext();
		    }
		


		} while (currentBlockID != 1); //&& writeBlockIDCounter != cap - 2

		if (currentBlockID == 1) {
			diskDrive->directory[dirCounter].setFirstBlockID(sizeDir);
			sizeDir += diskDrive->directory[dirCounter].getSize();
			dirCounter++;
			currentBlockID = diskDrive->directory[dirCounter].getFirstBlockID();
			
		} // If we hit the last block of a file

	} // for

	int stackSize = emptyStack.topOfStack;
	for (int i = -1; i < stackSize; i++) { //writing the empties, -1 bc stacksize starts at 0
		diskDrive->writeDiskBlock(arr[emptyStack.topAndPop()], --cap);
	}
	/*HT->makeEmpty();
	delete HT;*/


} // Defragmenter()














































//version 2

/*
#include "defragmenter.h"
#include "DefragRunner.h"
#include "mynew.h"
#include "QuadraticProbing.h"
#include "StackAr.h"
#include <iostream>

using namespace std;

Defragmenter::Defragmenter(DiskDrive* diskDrive)
{
	StackAr<int> emptyStack (50000);
	QuadraticHashTable<int>* HT;
	HT = new QuadraticHashTable<int> (-1, 200000);
	DiskBlock* arr[15000];
	int fileNum = diskDrive->getNumFiles();
	int cap = diskDrive->getCapacity();
	int currentBlockID = diskDrive->directory[0].getFirstBlockID();
	int sizeDir = 2; // file size counter, used for setting 1st block ID in directory
	int writeBlockIDCounter = 2; // file block number counter or index
	int dirCounter = 0; // directory counter
	int newLoc = 0;
	DiskBlock* dBlockPTR = NULL;
	DiskBlock* dBlockPTR2 = NULL;

	for (int i = 2; i < cap; i++) {
		if (diskDrive->FAT[i] == false) {  // Build stack of empties
			emptyStack.push(i);
	    }
	}






	for (int i = 0; i < fileNum; i++) { // for each file



		cout << "HERE1: " << endl;

		do {

			if (currentBlockID < writeBlockIDCounter) {
				dBlockPTR = arr[currentBlockID];
				cout << "currentBlockID: " << currentBlockID << endl;
				cout << "writeBlockIDCounter: " << writeBlockIDCounter << endl;
				if (dBlockPTR) { // if not NULL

				}
				else {
					newLoc = HT->find(currentBlockID);
					
					while (HT->find(newLoc) != -1) {
						HT->remove(currentBlockID);
						currentBlockID = newLoc;
						newLoc = HT->find(currentBlockID);
					}

					dBlockPTR = diskDrive->readDiskBlock(newLoc);
					if (newLoc != writeBlockIDCounter) {
						diskDrive->FAT[newLoc] = false;
						if (true) {
							emptyStack.push(newLoc);
						}
					}

					cout << "HERE3: " << endl;
				}
			} // if
			else { 
				dBlockPTR = diskDrive->readDiskBlock(currentBlockID);
		    }

		    if (currentBlockID != writeBlockIDCounter) {
		    	if (diskDrive->FAT[writeBlockIDCounter]) {
		    		dBlockPTR2 = diskDrive->readDiskBlock(writeBlockIDCounter);
		    	}

		    	diskDrive->writeDiskBlock(dBlockPTR2, emptyStack.top()); // write to biggest free spot
		    	HT->insert(writeBlockIDCounter, emptyStack.top());
		    	diskDrive->FAT[emptyStack.topAndPop()] = true;
		    	// delete dBlockPTR2;
		    }

			diskDrive->writeDiskBlock(dBlockPTR, writeBlockIDCounter);
			diskDrive->FAT[writeBlockIDCounter] = true;
			currentBlockID = dBlockPTR->getNext();
			//cout << "currentBlockID: " << currentBlockID << endl;
		    writeBlockIDCounter++;
		} while (currentBlockID != 1 && writeBlockIDCounter != cap - 2);

		if (currentBlockID == 1) {
			cout << "currentBlockID = 1" << endl;
			diskDrive->directory[dirCounter].setFirstBlockID(sizeDir);
			sizeDir += diskDrive->directory[dirCounter].getSize();
			dirCounter++;
			currentBlockID = diskDrive->directory[dirCounter].getFirstBlockID();
			
		} // If we hit the last block of a file

	} // for

	int stackSize = emptyStack.topOfStack;
	for (int i = -1; i < stackSize; i++) { //writing the empties, -1 bc stacksize starts at 0
		diskDrive->writeDiskBlock(arr[emptyStack.topAndPop()], --cap);
	}

	cout << "REACHED END" << endl;


} // Defragmenter()
*/
	/*for (int i = 2; i < cap; i++) { // Store as much of the DBlocks in RAM as possible
		dBlockPTR = diskDrive->readDiskBlock(i); 
		if ( i < ((int) sizeof(arr))/sizeof(arr[0])) { // read into array, maximize RAM
			arr[i] = dBlockPTR;
		}

		if (i >= ((int) sizeof(arr))/sizeof(arr[0])) { // read into hash 
			HT->insert(i, emptyStack.top()); // hash the outlying blockPTR to the biggest empty index, near the bottom
			diskDrive->writeDiskBlock(dBlockPTR, emptyStack.top());
			diskDrive->FAT[emptyStack.topAndPop()] = true;
		}*/

/*	diskDrive->writeDiskBlock(dBlockPTR2 , newLoc); 
diskDrive->FAT[writeBlockIDCounter] = true;
currentBlockID = dBlockPTR2->getNext();*/






















/*

#include "defragmenter.h"
#include "DefragRunner.h"
#include "mynew.h"
#include <cstdlib>

#include "QuadraticProbing.h"
#include "StackAr.h"

#define HASH_SIZE 300000
#define ARR_SIZE 100000


Defragmenter::Defragmenter(DiskDrive *diskDrive)
{
    DiskBlock *arr = new DiskBlock[ARR_SIZE];
    StackAr<int> empty(ARR_SIZE);

    int empty_index;

    DiskBlock* empty_block = diskDrive->readDiskBlock(1);
    empty_index = diskDrive->getCapacity();
    Item NOT_FOUND;
    NOT_FOUND.old_location = -1;
    NOT_FOUND.new_location = -1;

    for (int i = 0; i < ARR_SIZE; ++i) {
        empty.push(i);
    }

    QuadraticHashTable<Item> table(NOT_FOUND, HASH_SIZE);
    int current_block = 2;

    DiskBlock *to_delete;

    for (int num_file = 0; num_file < diskDrive->getNumFiles(); ++num_file) {
        DirectoryEntry file = diskDrive->directory[num_file];
        int size = file.getSize();
        int index = file.getFirstBlockID();

        diskDrive->directory[num_file].setFirstBlockID(current_block);

        for (int i = 0, b = current_block; i < size; ++b, ++i) {
            Item block;
            block.old_location = b;

            bool stuck = false;

            if ((to_delete = diskDrive->readDiskBlock(b))->getNext() != 0) {
                delete to_delete;
                // Make room
                Item result;
                if ((result = table.find(block)) == NOT_FOUND) {
                    if (empty.isEmpty()) {

                        while ((to_delete = diskDrive->readDiskBlock(empty_index))->getNext() != 0) {
                            delete to_delete;
                            empty_index--;
                            if (empty_index <= 1) {
                                empty_index = diskDrive->getCapacity();
                                if (stuck) {
                                    abort();
                                } else {
                                    stuck = true;
                                }
                            }
                        }

                        stuck = false;

                        Item loc;
                        loc.new_location = empty_index;
                        loc.old_location = b;
                        loc.in_arr = false;

                        table.insert(loc);
                    } else {
                        Item loc;
                        loc.new_location = empty.topAndPop();
                        loc.old_location = b;
                        loc.in_arr = true;

                        arr[loc.new_location] = *(to_delete = diskDrive->readDiskBlock(b));
                        delete to_delete;

                        table.insert(loc);
                    }
                    diskDrive->writeDiskBlock((to_delete = diskDrive->readDiskBlock(b)), empty_index);
                    delete to_delete;
                } else { //was found in the hash table, so already moved
                    while((to_delete = diskDrive->readDiskBlock(empty_index))->getNext() != 0) {
                        delete to_delete;
                        empty_index--;
                            if (empty_index <= 1) {
                                empty_index = diskDrive->getCapacity();
                                if (stuck) {
                                    abort();
                                } else {
                                    stuck = true;
                                }
                            }
                    }
                    stuck = false;

                    Item loc;
                    loc.old_location = result.old_location;
                    loc.new_location = empty_index;
                    loc.in_arr = false;

                    table.insert(loc);
                }
            }

            DiskBlock disk_block;
            Item temp;
            temp.old_location = index;
            if ((temp = table.find(temp)) == NOT_FOUND) {
                disk_block = *(to_delete = diskDrive->readDiskBlock(index));
                delete to_delete;
                diskDrive->writeDiskBlock(empty_block, index);
                if (index > empty_index) {
                    empty_index = index;
                }
            } else {
                if (temp.in_arr) {
                    disk_block = arr[temp.new_location];
                    empty.push(temp.new_location);

                } else {
                    disk_block = *(to_delete = diskDrive->readDiskBlock(index));
                    delete to_delete;
                    diskDrive->writeDiskBlock(empty_block, index);
                    if (index > empty_index) {
                        empty_index = index;
                    }
                    table.remove(temp);
                }

            }

            index = disk_block.getNext();
            if (disk_block.getNext() != 1) {
                disk_block.setNext(b + 1);
            }
            diskDrive->writeDiskBlock(&disk_block, b);

            current_block++;

        }
    }

    for (; current_block < diskDrive->getCapacity(); ++current_block) {
        diskDrive->writeDiskBlock(empty_block, current_block);
    }

    delete arr;
} // Defragmenter()

*/
