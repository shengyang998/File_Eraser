//
//  main.cpp
//  eraser
//
//  Created by Ysy on 2018/11/28.
//  Copyright © 2018 Ysy. All rights reserved.
//

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<ctime>


#define LL long long

const int buffer_size = 4096;

using namespace std;

LL getSizeOfFile(ofstream& file){
    file.seekp(0, file.end);
    LL sizeOfFile = file.tellp();
    file.seekp(0, file.beg);
    return sizeOfFile;
}

int toInt(char ch[]){
    int result = 0;
    string restr = ch;
    stringstream strStream(restr);
    if(!strStream){
        result = -1;
    }else{
        strStream >> result;
    }
    return result;
}

void initRandomArray(char arr[], const int len){
    for (int i=0; i<len; i++){
        srand((unsigned int)time(nullptr));
        int ra = rand();
        arr[i] = ra - (ra >> 6 << 6); // ra % 128;
//        cout<<"random number: "<<ra<<". random char: "<<arr[i]<<endl;
    }

}

int main(int argc, char* argv[]){
    cout<<"WARNING: The file will not be able to recover once it has been erased!"<<endl;
    cout<<"WARNING: !!!!!!     !!!!     !!!    !!!    !!!!!!    !!!!!!!!  !!!!!!"<<endl;
    cout<<"WARNING: !!  !!!   !!  !!    !! !   !!    !!   !!!   !!        !!   !!"<<endl;
    cout<<"WARNING: !!  !!!   !!  !!    !!  !  !!   !!          !!        !!   !!"<<endl;
    cout<<"WARNING: !!  !!!   !!!!!!    !!  !  !!   !!   !!!!!  !!!!!!!!  !!!!!!"<<endl;
    cout<<"WARNING: !!  !!!   !!  !!    !!   ! !!    !!    !!   !!        !!   !!"<<endl;
    cout<<"WARNING: !!!!!!   !!!  !!!  !!!    !!!     !!!!!!    !!!!!!!!  !!    !!"<<endl<<endl;
    // Prompt user to confirm
    cout<<"DO YOU WANT TO CONTINUE?[Y/N]: ";
    char yn = '\0';
    while(!(cin>>yn) || ((yn != 'y')&&(yn != 'Y')&&(yn != 'n')&&(yn != 'N'))){
        cout<<"Input error, please try again: ";
        cin.ignore(255,'\n');
        cin.clear();
    }
    if(yn == 'n' || yn == 'N'){
        cout<<"Done, nothing changed."<<endl;
        return -1;
    }
    // Confirmed
    string fileName;
    int times = -1;
    if (argc==1) {//zero para
        cout<<"Please input file name: ";
        while(!(cin>>fileName))
            cerr<<"Error in file name, please input again: "; cin.ignore(255, '\n'); cin.clear();
    }
    else if(argc > 1) {//get one parameter, assuming it it file name
        fileName = argv[1];
        if(argc > 2){//get two parameters, assuming it is file name and times
            times = toInt(argv[2]);
        }
    }
    if(times <= 0){
        cout<<"How much times you want to cover: ";
        while(!(cin>>times)){
            cerr<<"Error, please input again: "; cin.ignore(255, '\n'); cin.clear();
        }
    }
    //create the fileStream
    ofstream fileOut;
    //open the file
    fileOut.open(fileName, ios::app|ios::binary);
    fileOut.seekp(0, ios::beg);
    //get size of file
    LL sizeOfFile = getSizeOfFile(fileOut);
    cout<<"The size of the file is: "<<sizeOfFile<<endl;
    cout<<"The file will be covered "<<times<<" times."<<endl;
    cout<<"covering data..."<<endl;
    //prepare the data to cover
    char* atom = new char[1];
    
    for(int i=0; i<times; i++){
        //write the file
        fileOut.seekp(0, ios::beg);
        LL blocksOfFile = sizeOfFile>>12 ;// sizeOfFile/buffer_size;
        //write with content
        for(LL j=0; j < blocksOfFile; j += buffer_size){
            //delete the old content and alloc new content
            delete[] atom;
            atom = new char[buffer_size];
            initRandomArray(atom, buffer_size);
            fileOut.write(atom, buffer_size);
            // IF I SHOULD DELETE THE PLUS ONE?
            fileOut.seekp((j+1)*buffer_size);
            if(fileOut.tellp() == fileOut.end){
                cerr<<"break the loop with tellp == file.end"<<endl;
                break;
            }
        }
    }
    //close the file
    fileOut.close();
    
    //clean the file
    //fileOut.open(fileName);
    //fileOut.close();
    
    //clean the mem
    delete[] atom;
    atom = nullptr;
    
    //delete the file
    if(0 == remove(fileName.c_str())){
        cout<<"File is erased."<<endl;
    }else{
        perror("Error erasing file");
    }
    
    return 0;
}
