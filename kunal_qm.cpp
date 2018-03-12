#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <ctime>
using namespace std;
int MIN_BITS = 6;
int m=6;
const int RANGE = pow(2,MIN_BITS);
vector<unsigned> input;

////// we need to fix the code so that it does not go in the loop only twice///////
struct Boolean_number
	{
	unsigned number;
	unsigned dashes;
	char s[10000];
	bool flag;
	};
/////this structure is made to make the implementation of implicant primes easier

struct C
	{
	char list[10000];	
	long int minterm[10000];// this range is weird
	long int essential;
	};

vector<vector <Boolean_number> > starting_table;

// you probably need an array of this and you need to put in a loop for the processing as well as the display, which is done here using single calls lol
// that is a lot of work, but you can do it :)
// or maybe you can start with the array of vectors right now
vector<vector <Boolean_number> > table[100];
vector <Boolean_number>  unique;
C uni[10000];
C last[10000];

ofstream myfile3;
ofstream myfile;
///////////////////////////////////
unsigned count_ones(unsigned number); //count the number of 1s in a number
void print_binary(unsigned number);//print the number in binary
void create_table1();		//create original table sorted by the number of 1s
void print_table1();		//print the table
Boolean_number init_Boolean_number(unsigned n,long int d, bool u);//initialize a B_number
void create_p_group_general(long int a);		//create mid process table
void print_p_group_general(long int a);			//print it
void print_p_binary	(unsigned n, unsigned d);//print the mid table (with -'s)
void create_final_group();		//create final table
void print_final_group();		//print final table with -'s and unused terms	
void getinput1();		//get input from user
unsigned count_bits(unsigned n);	//min bits to represent a number
long int similarity_index(char a[], char b[]); // checks the similarity of two char arrays and return the value needed
void final_vector(); // changes to the second last stage (stage of prime implicants)
void goodjob(char a[],char b[] ,char c[]); // compares two constituents and introduces '-' if the condition satisfies
long int find_in(Boolean_number x);
long int matching_chars(char a[], char b[],long int ); // checking the similarity of two character arrays
void copying_chars(char src[], char dest[], long int offset); // copying one char to another
void display_final();// displays final one seperately for now
long int tobinary_equivalent(char a[], long int length );
void replace_dashes(char a[] , long int arr[]); // will call the following two functions recursively 
void replace_by_1(char a[], long int arr[]);// do not use the actual copy of the strings here pls
void replace_by_0(char a[], long int arr[]);// because you really dont want to fuck up with them lol
void init_all_C();
void init_C(C c); // to initiate all minterms to 0 , will be done one after the things are extracted
void display_uni_minterms(C c);// to display the minterms of  a particular C type
void display_all_uni_minterms();
void replace_all_uni_dashes();
void prime_to_essential(); // takes unique array and converts it to a the last array we need 
void change_row_to_zero();// function to change the whole row to zero in the array of uni and also designate essential to one
long int find_loner();// find the minterm that belongs to only one and change that to zero and then call change_row_to_zero function



	////////////////////////

int main(int argc, char *argv[]) {

	myfile.open("example.txt", std::ios_base::app);
	int start_s=clock();
	getinput1();
	create_table1();
	//print_table1();
	long int a=1;
	do
	{
	create_p_group_general(a);		
	//print_p_group_general(a);
	//cout<<"\n\n"<<a<<endl;
	a++;	
	}
	while(table[a-1].size()>0); // end of do while loop

	final_vector();
	display_final();
	
	init_all_C();
	replace_all_uni_dashes();
	//display_all_uni_minterms();	
	change_row_to_zero();	
	//display_all_uni_minterms();
	//cout<<"\nthese are the prime implicants \n";	
	//display_final();
	cout<<"\nESSENTIAL PRIME IMPLICANTS:  \n";
	
	prime_to_essential();
	
	int stop_s=clock();
	myfile<<"EXECUTION TIME: "<<(stop_s-start_s)/double(CLOCKS_PER_SEC)*10/5 << " seconds "<< endl;
	//myfile<<"PROBABILITY OF OCCURRENCE OF A MINTERM = 75 %" << endl;
	cout<<"EXECUTION TIME: "<<(stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
	//cout<<"PROBABILITY OF OCCURRENCE OF A MINTERM = 75 %" << endl;
	return 0;
	
}


//////////////all function definitions/////////



void prime_to_essential()
{

ofstream myfile2;
myfile2.open("example.txt", std::ios_base::app);
myfile2<<"\nESSENTIAL PRIME IMPLICANTS:  \n";
//myfile.open ("example.txt");
long int last_count=0;
for(long int i=0; i < 10000 ; i++)
	{
	if(uni[i].essential==1)
		{
		
		copying_chars(uni[i].list,last[last_count].list, MIN_BITS);
		myfile2<<last[last_count].list<<endl;		
		cout<<last[last_count].list<<endl; // maybe introduce a write to file function here to make things easier		
		last_count++;
		
		}			
	}
}


void change_row_to_zero()// function to change the whole row to zero in the array of uni and also designate essential to one
{
long int ret_val_loner;
	do
	{
	ret_val_loner=find_loner();
	
	}
	while(ret_val_loner!=-1);
}

long int find_loner()// find the minterm that belongs to only one and change that to zero and then call change_row_to_zero function
		//only finds the immediate first loner lol // this can also change the other elements to zero, but only for one iterartion,
		// also assign the correspoding uni element's essential flag to 1
{
long int count=0;
long int lone_flag=-1;
long int new_j=-1; // change this to the latest j value that holds the 1 value, for count=1, there will only be one j value
for (long int i=0; i < 10000; i++) // i is for the minterm
	{
	for(long int j=0;j < 10000;j++) // j is for all the uni10
		{
		if(uni[j].minterm[i]==1)
			{
			//cout<<count<<" ";
			count++;
			new_j=j;
			}
		}
	if(count==1)
		{
		// also turn the same minterms as zero for all
		// thanks lol		
		//cout<<"\n this is new_j \n";
		//cout<<new_j<<endl;
		uni[new_j].essential=1;
		lone_flag=i;
		break;
		}
	count=0;
	}
	
for(long int i=0; i< 10000 ; i++)
	{
	if(uni[new_j].minterm[i]==1)
		{
		for(long int j=0; j<10000;j++)
			{			
			uni[j].minterm[i]=0;
			}
		}		
	}
return lone_flag;

}
/////////////////////////////////////////////////

void init_all_C()
{
for(long int i=0;i<10000;i++)
	init_C(uni[i]);
}

void init_C(C c)
{
for (long int i=0; i< 10000 ; i++)
	{	
	c.minterm[i]=0;
	}
c.essential=0;	
}


///////////
void display_all_uni_minterms()
{
for (long int i=0;i<5;i++)
	{
	//cout<<uni[i].list<<"  ->  ";
	display_uni_minterms(uni[i]);
	}
}



void display_uni_minterms(C c)
{
for (long int i=0;i<50;i++)
	{
	//cout<<c.minterm[i]<<" ";
	}
//cout<<endl;
}




///////////////////////////////


void replace_all_uni_dashes()
{
for(long int i=0;i<10000;i++)
	{
	replace_dashes(uni[i].list, uni[i].minterm);
	}	
}



void replace_dashes(char a[], long int arr[])
{
long int dash_count=0;
char for_zero[MIN_BITS];
char for_one[MIN_BITS];
copying_chars(a,for_zero, MIN_BITS);
copying_chars(a,for_one, MIN_BITS);
// maybe i will need to create two more instances for the char array so that i am able to send different arrays in each function
for (long int i=0;i<MIN_BITS;i++)
	{
	if(a[i]=='-')
		{
		dash_count++;
		}
	}

if(dash_count>0)
	{
	replace_by_0(for_zero , arr);
	replace_dashes(for_zero , arr);
	}
if(dash_count>0)
	{
	replace_by_1(for_one , arr);
	replace_dashes(for_one , arr);
	}

}

void replace_by_1(char a[] , long int arr[])
{
long int dash_count=0;
char copy[MIN_BITS];// because i dont want to change the original string here
copying_chars(a,copy, MIN_BITS);
for (long int i=0;i<MIN_BITS;i++)
	{
	if(copy[i]=='-')
		{
		copy[i]='1';
		break;
		}
	}
for (long int i=0;i<MIN_BITS;i++)
	{
	if(copy[i]=='-')
		{
		dash_count++;
		}
	}

if(dash_count==0)
	{
	// another line to put that into its minterm list lol
	arr[tobinary_equivalent(copy,MIN_BITS)]=1;
	//cout<<copy<<endl;
	}
copying_chars(copy,a, MIN_BITS);

//return dash_count;

}


void replace_by_0(char a[] ,long  int arr[])
{
long int dash_count=0;
char copy[MIN_BITS];// because i dont want to change the original string here
copying_chars(a,copy, MIN_BITS);
for (long int i=0;i<MIN_BITS;i++)
	{
	if(copy[i]=='-')
		{
		copy[i]='0';
		break;
		}
	}
for (long int i=0;i<MIN_BITS;i++)
	{
	if(copy[i]=='-')
		{
		dash_count++;
		}
	}

if(dash_count==0)
	{
	arr[tobinary_equivalent(copy,MIN_BITS)]=1;
	tobinary_equivalent(copy,MIN_BITS);
	//cout<<copy<<endl;
	}

copying_chars(copy,a, MIN_BITS);
//return dash_count;

}


// keep using replace by 0 and replace by 1 untill there are no dashes remaining
// else print them 
// good boy, good ideas


long int tobinary_equivalent(char a[], long int length )
	{
	long int result=0;
	for(long int i=0; i < length ; i++)
		{
		if(a[i]=='0')
			result=result+0;
		else if (a[i]=='1')
			result=result+ pow(2,i);
		}
	//cout<<result<<endl;
	return result;
	}

void copying_chars(char src[], char dest [], long int offset)  //copies source into destination
	{
	for(long int i=0;i<offset;i++)
		{
		dest[i]=src[i];
		}
	}

long int matching_chars(char a[] , char b[] ,long int limit)    // returns one if all the chars are matching
	{	
	long int count=0;
	for (long int i=0;i<limit;i++)
		{
		if(a[i]==b[i])
		count++;
		}
	if (count==limit)
		return 1;
	else
		return 0;
	}

long int similarity_index(char a[],char b[])
	{
	long int index=0;
	for (long int i=0;i<MIN_BITS;i++)
		{
		if(a[i]!=b[i])
			index++;
		}
	return index;
	}

void goodjob(char a[],char b[],char c[])
	{
	
	for(long int i=0;i<MIN_BITS;i++)
		{
		if(a[i]!=b[i])
			c[i]='-';
		else c[i]=a[i];
		}	
	
	}


void getinput1() {
	ifstream infile;
	infile.open("example.txt");
	
	long int in	;
	long int num_bits=0;
	if(infile.fail())
		{
		//cout<<"error"<<endl;
		}

	while(infile >> in) {
		
		input.push_back(in);
		num_bits = count_bits(in);	
		if(num_bits>m)
			m= num_bits;
		//cout<<"  "<<in<<"\n";
		//cout<<"> ";
		in;
		//cout<<"\n"<<in;
	}
	infile.close();
}



unsigned count_bits(unsigned n) {
	short bit =0;
	long int count = 0;
	while(n>0) {
		bit = n%2;
		n>>=1;
		count++;
	}
	return count;
}


void create_table1() {
	short tmp;
	Boolean_number temp_num;
	for(long int i=0;i<input.size();i++) {
		tmp = count_ones(input[i]);
		if(tmp+1>table[0].size())
			table[0].resize(tmp+1);
		
		temp_num = init_Boolean_number(input[i],0,false);
		table[0][tmp].push_back(temp_num);
	}
}


Boolean_number init_Boolean_number(unsigned	n,long int d, bool u) {
	Boolean_number num;
	num.number = n;
	num.dashes = d;
	num.flag = u;
	unsigned bits[MIN_BITS];
	long int count = 0;
	
	while(n>0||count<MIN_BITS)	{
		
		if(n%2==0){
			num.s[count]='0';	
			}
		else if (n % 2 == 1){	
			num.s[count]='1';
			}
		n>>=1;
		count++;	
		} // while ends
	return num;
}

void print_table1() {
	
	//cout<<endl<<"COMPUTING:"<<endl;
	for(long int i=0;i<table[0].size();i++)	{
	//	cout<<i;
		for(long int j=0;j<table[0][i].size();j++) {
	//		cout<<"\tm"<<table[0][i][j].number<<"\t";
			//print_binary(table[0][i][j].number);
			//cout<<"  this is the string now  .......   ";
	//		cout<<table[0][i][j].s;
	//		cout<<endl;
		}
	//	cout<<"\n-------------------------------------"<<endl;
	}
}


unsigned count_ones(unsigned number) {
	short bit =0;
	long int count = 0;
	while(number>0)	{
		bit = number%2;
		number>>=1;
		if(bit) {
			count++;
		}
	}
	return count;
}

void print_binary(unsigned number) {
	unsigned bits[MIN_BITS];
	long int count = 0;
	
	while(number>0||count<MIN_BITS)	{
		bits[count] = number%2;
		number>>= 1;
		count++;
	}
	//for(long int i=count-1;i>=0;i--)
	//	cout<<bits[i];
}



void print_p_binary(unsigned n, unsigned d) {
	unsigned bits[MIN_BITS];
	long int count = 0;
	
	while(n>0||count<MIN_BITS) {
		if(!(d%2))
			bits[count] = n%2;
		else
			bits[count] = 2;
		
		n >>= 1;
		d >>= 1;
		count++;
	}
	//for(long int i=count-1;i>=0;i--) {
	//	if(bits[i]!=2)
	//		cout<<bits[i];
	//	else
	//		cout<<"-";
	//}
}


//////////////////////////declaration for arguement dependent variable for making table is declared after this point/////////////
////////////////////////// tresspassers will be prosecuted//////////////////////////////


void create_p_group_general(long int a) {
	short tmp;
	Boolean_number temp_num;
	unsigned temp_number, temp_dashes;
	for(long int i=0;i<table[a-1].size()-1;i++) {
		for(long int j=0;j<table[a-1][i].size();j++) {
			for(long int k=0;k<table[a-1][i+1].size();k++) {
				temp_number = table[a-1][i][j].number & table[a-1][i+1][k].number;
				temp_dashes = table[a-1][i][j].number ^ table[a-1][i+1][k].number;
			
				if(similarity_index(table[a-1][i][j].s, table[a-1][i+1][k].s)==1) {	   // not present in the original	

					table[a-1][i][j].flag = true;
					table[a-1][i+1][k].flag = true;
					
					
					tmp = count_ones(temp_number);
					
					if(tmp+1>table[a].size())
						table[a].resize(tmp+1);
					
					temp_num = init_Boolean_number(temp_number, temp_dashes, false);
					goodjob(table[a-1][i][j].s, table[a-1][i+1][k].s,temp_num.s);
					table[a][tmp].push_back(temp_num);
					//goodjob(table[0][i][j].s, table[0][i+1][k].s,table[1][tmp].s);
				}
	
	
	
			}
		}
	}
}



void print_p_group_general(long int a) {
	//cout<<endl<<"MID PROCESS COMPUTATION:"<<endl;
	
	for(long int i=0;i<table[a].size();i++) {
		//cout<<i;
		for(long int j=0;j<table[a][i].size();j++) {
			//cout<<"\t\t";
			//print_p_binary(table[a][i][j].number,table[a][i][j].dashes);
		//	cout<<" "<<table[a][i][j].s;
		//	cout<<" "<<table[a][i][j].flag;
		//	cout<<endl;
		}
		//cout<<"\n-------------------------------------"<<endl;
	}
	
}


void final_vector()
{
long int init_flag=0;
long int u_count=0;
long int detect_match=0;// for the last loop
//cout<<"\n  cumulatuve final print \n";
for (long int a =0; a<10 ; a++) //loop 1
	{
	for(long int i=0;i<table[a].size();i++) // loop 2
		{
//		cout<<i;
		for(long int j=0;j<table[a][i].size();j++) // loop 3 
			{
			if(table[a][i][j].flag==0)	// condition one, this will reduce the number of comparisions that are needed		
						// so only compare if the used flag is set to 0, else don't lol, obvio	
						// firstly dont make a special case for the first iteration			
				{
				detect_match=0;
				for(long int k=0;k<10000;k++) // less than the size of 'uni'
					{
					detect_match=matching_chars(table[a][i][j].s, uni[k].list,MIN_BITS);
					//cout<<".... "<<detect_match<<endl;

					if(detect_match==1)
						break;

					// dont do anything, make a dummy run first in that main
					// maybe write another function to compare iteratively
					}
				if(detect_match==0)
					{
					copying_chars(table[a][i][j].s, uni[u_count].list,MIN_BITS);
						 // index should be u_count here for the destination
					u_count++;
					//cout<<"u_count is "<< u_count<<endl;
//					cout<<uni[u_count].list;
//					cout<<endl;
					}				
				}
			}
//		cout<<"\n-------------------------------------"<<endl;
		}
	
	}
}


void display_final()
{
//cout<<"FINAL DISPLAY IS HERE, BRACE YOURSELF"<<endl;
//for (long int i=0;i<10000;i++)
//	cout<<uni[i].list<<endl;
}
// now i need to iterate through all the table vectors and store them in the structure c if they are not already there
// for that i need to compare the string for both of them
// so basically for each iteration in the table vector, i will compare it through all the data in the structure C
// name that structure as the C final



// both the functions for reference
//int matching_chars(char a[] , char b[] ,int limit)    // returns one if all the chars are matching
//void copying_chars(char src[], char dest [], int offset)  //copies source into destination
// name of the structure is c



