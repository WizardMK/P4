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
	//int arrCounter = 0;
	int fileNum = diskDrive->getNumFiles();
	int cap = diskDrive->getCapacity();
	int currentBlockID = diskDrive->directory[0].getFirstBlockID();
	int sizeDir = 2; // file size counter, used for setting 1st block ID in directory
	int writeBlockIDCounter = 2; // file block number counter
	int dirCounter = 0; // directory counter
	int newPosHT = 0;
	DiskBlock* dBlockPTR = NULL;
	DiskBlock* dBlockPTR2 = NULL;

	//int currentFileID = diskDrive->directory[dirCounter].getFileID(); // Look at this directory

	for (int i = 2; i < cap; i++) {
		if (diskDrive->FAT[i] == false)  // Build stack of empties
			emptyStack.push(i);
	}

	for (int i = 2; i < cap; i++) { // Store as much of the DBlocks in RAM as possible
		dBlockPTR = diskDrive->readDiskBlock(i); 
		if ( i < 14599) { // read into array, maximize RAM
			arr[i] = dBlockPTR;
		}

		if (i >= 14599) { // read into hash 
			HT->insert(i, emptyStack.topAndPop()); // hash the outlying blockPTR to the biggest empty index, near the bottom
			cout << i << endl;
		}
	}


	for (int i = 0; i < fileNum; i++) { // for each file

		do {
			if (currentBlockID > 14599) {
				cout << "Greater than 14599" << endl;
				newPosHT = HT->find(currentBlockID);
				cout << "NEWPOSTHT: " << newPosHT << endl;
				cout << "currentBlockID: " << currentBlockID << endl;
				dBlockPTR2 = diskDrive->readDiskBlock(newPosHT);

				diskDrive->writeDiskBlock(dBlockPTR2 , emptyStack.top()); // move dBlock to furthest down empty spot
				diskDrive->FAT[emptyStack.topAndPop()] == true;
				//diskDrive->writeDiskBlock( , currentBlockID);
			}
			else { // currentBlockID <= 14599, within array
				diskDrive->writeDiskBlock(arr[currentBlockID], writeBlockIDCounter);
				//arr[currentBlockID]->setNext(writeBlockIDCounter );
				diskDrive->FAT[i] = true;
				cout << "writeBlockIDCounter: " << writeBlockIDCounter << " ";
				cout << "currentBlockID: " << currentBlockID << endl;
				writeBlockIDCounter++;
		    }
		
			currentBlockID = arr[currentBlockID]->getNext();


		} while (currentBlockID != 1 && writeBlockIDCounter != cap - 2);

		if (currentBlockID == 1) {
			diskDrive->directory[dirCounter].setFirstBlockID(sizeDir);
			sizeDir += diskDrive->directory[dirCounter].getSize();
			dirCounter++;
			currentBlockID = diskDrive->directory[dirCounter].getFirstBlockID();
			
		} // If we hit the last block of a file

	} // for
	int stackSize = emptyStack.topOfStack;
	for (int i = 0; i < stackSize; i++) { //writing the empties
		diskDrive->writeDiskBlock(arr[emptyStack.topAndPop()], --cap);
	}
	/*HT->makeEmpty();
	delete HT;*/
	cout << "REACHED END" << endl;


} // Defragmenter()

