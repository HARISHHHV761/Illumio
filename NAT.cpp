/*  Author : Harish Holenarsipura Viswanatha 
    Project: Checking the flow entry against the NAT entry
    Date: November 11th 2017    */


#include <fstream>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

unordered_map<string,string> mymap;

string split_flow_entry(string flow,int port_not_mentioned)
{
    string str1;
    int count=0;
    string final_string;
    string wrong_string="Wrong ip format";
    typedef vector< string > split_vector_type;
    split_vector_type flow_entry;
    split( flow_entry, flow, is_any_of(":"));
    for (vector<string>::const_iterator i = flow_entry.begin(); i != flow_entry.end(); ++i)
    {
        count++;
        //Check for entries like 10.0.1.1:* 
        if(count==1 && port_not_mentioned)
        {
            final_string=*i;
            final_string=final_string+":*";
            return final_string; 
        }
        //Check for entries like *:8080 
        if(count==2)
        {
            final_string=*i;
            final_string="*:"+final_string;
            return final_string;
        }    
    }
    return wrong_string;
}
void reading_entry()
{

    string line;
    string ip_port1;
    string ip_port2;
    string sep_port;
    // Read data from file 
    ifstream NAT_file("NAT1.txt");
    while(getline(NAT_file,line))
    {
        //cout <<"\n"<<line;
        istringstream sep(line);
        getline(sep, ip_port1,',');
        getline(sep, ip_port2,',');
        pair<string,string> my_nat (ip_port1,ip_port2);
        mymap.insert(my_nat);
    }
    for (pair<string, string> element : mymap)
    {
        cout << element.first << ":" << element.second <<endl;
    }
    cout <<"\n";
}
void search_flow_entry()
{
    string flow;
    ifstream flow_entry("flow1.txt");
    string formatted_string; 
    //Parsing the NAT file line by line
    while(getline(flow_entry,flow))
    {
        unordered_map<string,string>::iterator flowentry = mymap.find (flow);
        //No flow entry has matched the key value of NAT ENTRY table 
        if ( flowentry == mymap.end())
        {
            //The flow entry formated(10.1.1.1:80-> 10.1.1.1:*)
            formatted_string=split_flow_entry(flow,0);
            unordered_map<string,string>::iterator flowentry2 = mymap.find (formatted_string);
            if(flowentry2 == mymap.end())
            {
                //The flow entry formated(10.1.1.1:80-> *:80)
                formatted_string=split_flow_entry(flow,1);
                unordered_map<string,string>::iterator flowentry3 = mymap.find(formatted_string);
                if(flowentry3 == mymap.end())
                {
                    cout <<"No NAT found for -> "<<flow;
                }
                else
                {
                    cout << flow << " -> " << flowentry3->second;
                }
            }
            else
            {
                cout << flow << " -> " << flowentry2->second;
            }
        }
        //Flow entry has matched the key value of NAT ENTRY table without formating  
        else
            cout << flowentry->first << " -> " << flowentry->second;
        cout << std::endl;
    } 
}
int main() 
{
    reading_entry();
    search_flow_entry();
    return 0;

}