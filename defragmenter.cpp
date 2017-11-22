#include "defragmenter.h"
#include "DefragRunner.h"
#include "mynew.h"
#include "QuadraticProbing.h"
#include "QueueAr.h"
#include <iostream>

using namespace std;

Defragmenter::Defragmenter(DiskDrive* diskDrive)
{
	Queue<int> emptyQueue (100000);
	QuadraticHashTable <int> HT(-1, 200000);
	DiskBlock* arr [16000];
	int arrCounter = 0;
	int fileNum = diskDrive->getNumFiles();
	int cap = diskDrive->getCapacity();
	int currentBlockID = diskDrive->directory[0].getFirstBlockID();
	int sizeDir = 2; // file size counter, used for setting 1st block ID in directory
	int writeBlockIDCounter = 2; // file block number counter
	int dirCounter = 0; // directory counter
	int dequeuedEmpty = 0; // stores returned dequeued value of the empties
	//int emptyBlock = 0;
	DiskBlock* dBlockPTR = NULL;

	int currentFileID = diskDrive->directory[dirCounter].getFileID(); // Look at this directory

	for (int i = 2; i < cap; i++) { // Store as much of the DBlocks in RAM as possible
		dBlockPTR = diskDrive->readDiskBlock(i); 
		arr[i] = dBlockPTR;

		if (diskDrive->FAT[i] == false) {
			emptyQueue.enqueue(i);
			cout << "ENQUEUED: " << i << endl;
		}
	/*	if (dBlockPTR->getFileBlockNum() == 0) {

		}*/

	}

	for (int i = 0; i < fileNum; i++) { // for each file

		do {

			diskDrive->writeDiskBlock(arr[currentBlockID], writeBlockIDCounter);
			//arr[currentBlockID]->setNext(writeBlockIDCounter );
			diskDrive->FAT[i] = true;
			cout << "writeBlockIDCounter: " << writeBlockIDCounter << " ";
			cout << "currentBlockID: " << currentBlockID << endl;
			writeBlockIDCounter++;
		
			currentBlockID = arr[currentBlockID]->getNext();

		} while (currentBlockID != 1 && writeBlockIDCounter != cap - 2);

		if (currentBlockID == 1) {
			diskDrive->directory[dirCounter].setFirstBlockID(sizeDir);
			sizeDir += diskDrive->directory[dirCounter].getSize();
			dirCounter++;
			currentBlockID = diskDrive->directory[dirCounter].getFirstBlockID();
			
		} // If we hit the last block of a file
	
	} // for
	
	int queueSize = emptyQueue.currentSize;
	for (int i = 0; i < queueSize; i++) { //writing the empties
		diskDrive->writeDiskBlock(arr[emptyQueue.dequeue()], --cap);
	}



} // Defragmenter()

