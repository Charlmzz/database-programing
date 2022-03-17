#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <pqxx/pqxx>

#include "exerciser.h"

using namespace std;
using namespace pqxx;

void createTables(string db, connection *C){
  work W(*C);
  W.exec(db);
  W.commit();
}

void initializeColor(connection *C){
  ifstream inFile("color.txt");
  string line;

  while (getline(inFile,line)){
    int color_id;
    string name;
    istringstream ss(line);
    ss >> color_id >> name;
    add_color(C,name);
  }
}

void initializeState(connection *C){
  ifstream inFile("state.txt");
  string line;

  while (getline(inFile,line)){
    int state_id;
    string name;
    istringstream ss(line);
    ss >> state_id >> name;
    add_state(C,name);
  }
}

void initializeTeam(connection *C){
  ifstream inFile("team.txt");
  string line;

  while (getline(inFile,line)){
    int team_id, color_id,state_id,wins,losses;
    string name;
    istringstream ss(line);
    ss >> team_id >> name >> state_id >> color_id >> wins >> losses;
    add_team(C,name,state_id,color_id,wins,losses);
  }
}

void initializePlayer(connection *C){
  ifstream inFile("player.txt");
  string line;

  while (getline(inFile,line)){
    int player_id,team_id,uniform_num,MPG,PPG,RPG,APG;
    double SPG,BPG;
    string first_name,last_name;
    istringstream ss(line);
    ss >> player_id >> team_id >> uniform_num >> first_name >> last_name >> MPG >> PPG >> RPG >> APG >> SPG >> BPG;
    add_player(C,team_id,uniform_num,first_name,last_name,MPG,PPG,RPG,APG,SPG,BPG);
  }
}

void initializeAll(connection *C){
  initializeColor(C);
  initializeState(C);
  initializeTeam(C);
  initializePlayer(C);
}

int main (int argc, char *argv[]) 
{

  //Allocate & initialize a Postgres connection object
  connection *C;

  try{
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      cout << "Opened database successfully: " << C->dbname() << endl;
    } else {
      cout << "Can't open database" << endl;
      return 1;
    }
  } catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return 1;
  }


  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  //      load each table with rows from the provided source txt files
  ifstream inFile("myDB.sql");
  stringstream strStream;
  strStream << inFile.rdbuf();
  string db = strStream.str();
  createTables(db,C);
  
  initializeAll(C);

  exercise(C);


  //Close database connection
  C->disconnect();

  return 0;
}


