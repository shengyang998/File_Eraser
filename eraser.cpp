#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

const int buffer_size = 4096;

using namespace std;

long long getSizeOfFile(ofstream& file){
    file.seekp(0, file.end);
    long long sizeOfFile = file.tellp();
    file.seekp(0, file.beg);
    return sizeOfFile;
}

int setContent(char content[], int size){
    for(int i=0;i<size;i++){
        //content[i] = content[i]*content[i+size]*(~content[i+2*size]);
        content[i] = content[i+size] & content[i+2*size];
    }
    return 0;
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
            cout<<"Error in file name, please input again: ";cin.ignore(255, '\n');cin.clear();
    }
    else if(argc > 1) {//one para, assume it as file name
        fileName = argv[1];
        if(argc > 2){//two paras, assume it as file name and times
            times = toInt(argv[2]);
        }
    }
    if(times <= 0){
        cout<<"How much times you want to cover: ";
        while(!(cin>>times)){
            cout<<"Error, please input again: ";cin.ignore(255, '\n');cin.clear();
        }
    }
    //create the fileStream
    ofstream fileOut;
    //open the file
    fileOut.open(fileName, ios::app|ios::binary);
    fileOut.seekp(0,ios::beg);
    //get size of file
    long long sizeOfFile = getSizeOfFile(fileOut);
    cout<<"The size of the file is: "<<sizeOfFile<<endl;
    cout<<"The file will be covered "<<times<<" times."<<endl;
    cout<<"covering data..."<<endl;
    //prepare the data to cover
    char* content = new char[1];

    for(int i=0;i<times;i++){
        //write the file
        fileOut.seekp(0, fileOut.beg);
        //write with content
        for(long long j=0;j<(sizeOfFile/buffer_size);j+=buffer_size){
            //delete the old content and alloc new content
            //delete[] content;
            content = new char[buffer_size];
            //setContent(content, buffer_size);

            fileOut.write(content, buffer_size);
            // IF I SHOULD DELETE THE PLUS ONE?
            fileOut.seekp((j+1)*buffer_size);
            if(fileOut.tellp() == fileOut.end){
                break;
            }
        }
    }
    //close the file
    fileOut.close();

    //clean the file
    //fileOut.open(fileName);
    fileOut.close();

    //clean the mem
    delete[] content;
    content = nullptr;

    //delete the file
    if(0 == remove(fileName.c_str())){
        cout<<"File is erased."<<endl;
    }else{
        perror("Error erasing file");
    }

    return 0;
}
