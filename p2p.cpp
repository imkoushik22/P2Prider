#include<bits/stdc++.h>
#include<sys/time.h>
#include "picosha2.h"
using namespace std;

class Block
{
    public:
    string hash;
    string previousHash;
    Block(string data1,string previousHash1 ) {
		data = data1;
		previousHash = previousHash1;
		struct timeval tp;
        gettimeofday(&tp, NULL);
        timeStamp = tp.tv_sec * 1000 + tp.tv_usec / 1000;
		hash = calculateHash(); //Making sure we do this after we set the other values.
	}
    Block(const Block &b)
    {
        hash=b.hash;
        data=b.data;
        previousHash=b.previousHash;
        timeStamp=b.timeStamp;
    } 
	//Calculate new hash based on blocks contents
	string calculateHash() {
		string calculatedhash;
        string src=previousHash +
				to_string(timeStamp) +
				to_string(nonce) + 
				data;
        picosha2::hash256_hex_string(src,calculatedhash);
		return calculatedhash;
	}
	void mineBlock(int difficulty) {
		string target;
        for(int i=0;i<difficulty;i++)
        target.push_back('0'); 
        //Create a string with difficulty * "0" 
		while(hash.substr(0, difficulty)!=target) {
			nonce ++;
			hash = calculateHash();
		}
		cout<<"Block Mined!!! : "<<hash;
	}
    private:
    long int timeStamp;
    int nonce;
    string data;
};
struct rider
{
    string name;
    string userid;
    string source;
    string destination;
    string cartype;
    int price;
    bool pooled;
    string ownerid;
};
struct owner
{
    int seats;
    string cartype;
    string source;
    string destination;
    int price;
    string name;
    string userid;
    set<string> riderid;
};
vector<Block> blockchain;
int difficulty=2;
bool isChainValid(int difficulty) {
		string hashTarget;
        for(int i=0;i<difficulty;i++)
        hashTarget.push_back('0');
		
		//loop through blockchain to check hashes:
		for(int i=1; i < blockchain.size(); i++) {
			
			//compare registered hash and calculated hash:
			if(blockchain[i].hash!=blockchain[i].calculateHash()){
				cout<<"Current Hashes not equal";			
				return false;
			}
			//compare previous hash and registered previous hash
			if(blockchain[i-1].hash!=blockchain[i].previousHash)  {
				cout<<"Previous Hashes not equal";
				return false;
			}
			//check if hash is solved
			if(blockchain[i].hash.substr(0,difficulty)!=hashTarget) {
				cout<<"This block hasn't been mined";
				return false;
			}
		}
		return true;
	}
int main()
{
    /*vector<rider> r;
    vector<owner> o;
    set<string> userid;
    cout<<"Welcome to P2P ridesharing:\n";
    cout<<"1.Rider\n2.Owner\n3.Exit\nChoose an option:";
    int option;
    while(1)
    {
        cin>>option;
        if(option==1)
        {
            struct rider newrider;
            newrider.pooled=false;
            int x;
            string y;
            cin>>y;
            newrider.name=y;
            cin>>y;
            while(userid.find(y)!=userid.end())
            {
                cout<<"Username not available, try again:";
                cin>>y;
            }
            s.insert(y);
            newrider.userid=y;
            cin>>y;
            newrider.source=y;
            cin>>y;
            newrider.destination=y;
            cin>>y;
            newrider.cartype=y;
            cin>>x;
            newrider.price=x;
            cout<<"Searching for potentials riders......\n";
        }
        else if(option==2)
        {
            
        }
        else if(option==3)
        {
            break;
        }
        else
        {
            cout<<"Invalid option\nEnter again:";
        }
    }*/
    Block b("Hi im the first block","0");
    blockchain.push_back(b);
	cout<<"Trying to Mine block 1... ";
	blockchain[0].mineBlock(difficulty);
    
    Block c("Hi im the second block",blockchain[0].hash);
    blockchain.push_back(c);
	cout<<"Trying to Mine block 1... ";
	blockchain[1].mineBlock(difficulty);
    cout<<isChainValid(difficulty);
    
    return 0;
}