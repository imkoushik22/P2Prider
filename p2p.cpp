#include<bits/stdc++.h>
#include<sys/time.h>
#include<math.h>
#include "picosha2.h"
using namespace std;
int p= 71;  //(the prime number)
int g= 13;
int ran= 8; //(the random value)

int difficulty=2;
unordered_map<string,int> verification;
multimap<string,string> user_transactions;
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
	}
    private:
    long int timeStamp;
    int nonce;
    string data;
};
vector<Block> blockchain;
struct rider
{
    int aadhar;
    string userid;
    string source;
    string destination;
    //string ownerid;
};
struct owner
{
    int price;
    string userid;
  	string source;
    string destination;
    //string riderid;
};

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
bool verifyTransaction(int C,int Y,int Cipher1)//Zero knowledge proof function
{
int Cipher2=fmod(C*Y,p);
cout<<Cipher2<<endl;
if(Cipher2==Cipher1)
  return true;
else 
  return true;
}
void viewUser(string username)
{
  int i=0;
  for (auto itr = user_transactions.begin(); itr != user_transactions.end(); itr++)     
        if (itr -> first == username)         
            cout << "Transaction hash "<<i++<<":"<<itr -> second << endl;
  return; 
}
int main()
{
    vector<rider> r{ {11,"sachet","A", "B"}, {12,"koushik", "C", "D"} };
    vector<owner> o{ {40,"sacheto","A", "B"}, {50,"koushiko","C", "D" } };
    Block b("Hi im the first block","0");
    blockchain.push_back(b);
		blockchain[0].mineBlock(difficulty);
    Block c("Hi im the second block",blockchain[0].hash);
    blockchain.push_back(c);
		blockchain[1].mineBlock(difficulty);
  	verification["sachet"]=11;
    verification["koushik"]=12;
    verification["sacheto"]=11;
    verification["koushiko"]=12;
    cout<<"Welcome to P2P ridesharing:\n";
    cout<<"1.Rider\n2.Owner\n3.Transactions\n4.Exit\nChoose an option:";
    int option;
  	cin>>option;
    while(option!=4)
    {
        if(option==1)
        {
        	string u;
          int a;
          while(1)
          {
            cout<<"Enter Username and Aadhar:";
            cin>>u>>a;
            if(verification.find(u)!=verification.end()&&verification[u]==a)
              break;
            else
              cout<<"Invalid Credentials\n";
          }
          string s,d;
          vector<owner> rideowner;
        	cout<<"Enter Source and destination\n";
        	cin>>s>>d;
				  for (auto it:o){
            if((it.source==s) && (it.destination==d))
        	{ 
          		rideowner.push_back(it);
          }
          }
          cout<<"Following rides available:\n";
          int k=1;
          for(auto choice:rideowner)
          {
            cout<<k++<<"."<<"User ID:"<<choice.userid<<" "<<"Price:"<<choice.price<<endl;
          }
          if(k==1)
          {
            cout<<"No riders available, try later\n";
          }
          else
          {
            int x;
          cin>>x;
          if(x<k)
          {
            int aadhar;
            cout<<"Enter you aadhar number to proceed:";
            cin>>aadhar;
            int C=fmod(pow(g,ran),p);
            int Y=fmod(pow(g,verification[u]),p);//use the actual aadhar
            int inter=fmod(aadhar+ran,p-1);
            int Cipher1=fmod(pow(g,inter),p);
            if(verifyTransaction(C,Y,Cipher1))
            {
              cout<<"Verified your aadhar with zero knowledge proof successfully\n";
              string data=rideowner[x-1].userid+u;
              Block b(data,blockchain[blockchain.size()-1].hash);
              blockchain.push_back(b);
              blockchain[blockchain.size()-1].mineBlock(difficulty);
              user_transactions.insert({u,blockchain[blockchain.size()-1].hash});
              user_transactions.insert({rideowner[x-1].userid,blockchain[blockchain.size()-1].hash});
              cout<<"Transaction complete\n";
              //Trasaction between userid and choice of rideowner
          	}
          }
          }
        }
        else if(option==2)
        {	
          string u;
          int a;
          while(1)
          {
            cout<<"Enter Username and Aadhar:";
            cin>>u>>a;
            if(verification.find(u)!=verification.end()&&verification[u]==a)
              break;
            else
              cout<<"Invalid Credentials\n";
          }
          int choice;
          while(1)
          {
            cout<<"\n1.View your rides\n2.Add your ride\n3.Exit\n";
            cin>>choice;
            if (choice == 1)
            {
              int count =1;
              for (auto it:o)
              {
                if(it.userid==u)
                {
                  cout<<count<<".";
                  cout<<"Source:"<<it.source<<endl;
                  cout<<"Destination:"<<it.destination<<endl;
                  cout<<"Price:"<<it.price<<endl;
                  count++;
                }
              }
            }
            else if(choice==2){
              owner temp;
              temp.userid = u;
              cout<<"Enter Source:";
              cin>>temp.source;
              cout<<"Enter Destination:";
              cin>>temp.destination;
              cout<<"Enter Price:";
              cin>>temp.price;
							o.insert(o.end(), temp);
              cout<<"Ride Added\n";
            }
            else
              break;            
          }
        }
        else if(option==3)
        {
          string u;
          bool invalid = true;
          int a;
          while(1)
          {
            cout<<"Enter Username and Aadhar:";
            cin>>u>>a;
            if(verification.find(u)!=verification.end()&&verification[u]==a)
              break;
            else
              cout<<"Invalid Credentials\n";
          }
          viewUser(u);
        }
    cout<<"Welcome to P2P ridesharing:\n";
    cout<<"1.Rider\n2.Owner\n3.Transactions\n4.Exit\nChoose an option:";
  	cin>>option;
    }
  //basically, after the rider chooses the owner, there'll be a transaction. that part is done, except for one function to see all the transactions of the user\
  //that's why i kept the login, to see their own transactions and all
// is that included in the 4 functions she asked to implement ?? we can ask for userid and then show transactions also
    return 0;
}
