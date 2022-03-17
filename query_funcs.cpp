#include "query_funcs.h"


void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg)
{
    work W(*C);
    stringstream ss;
    ss << "INSERT INTO PLAYER (TEAM_ID,UNIFORM_NUM,FIRST_NAME,LAST_NAME,MPG,PPG,RPG,APG,SPG,BPG) VALUES (" << team_id << "," << jersey_num << "," << W.quote(first_name) << "," <<W.quote(last_name) <<","<< mpg<<"," <<ppg<<","<< rpg <<"," <<apg << ","<<spg <<","<<bpg <<");";
    string sql = ss.str();
    W.exec(sql);
    W.commit();
}


void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses)
{
    work W(*C);
    stringstream ss;
    ss << "INSERT INTO TEAM (NAME,STATE_ID,COLOR_ID,WINS,LOSSES) VALUES (" << W.quote(name) << "," << state_id << "," << color_id << "," <<wins <<","<<losses<<");";
    string sql = ss.str();
    W.exec(sql);
    W.commit();
}


void add_state(connection *C, string name)
{
    work W(*C);
    string sql = "INSERT INTO STATE (NAME) VALUES ("+W.quote(name)+");";
    W.exec(sql);
    W.commit();
}


void add_color(connection *C, string name)
{
    work W(*C);
    string sql = "INSERT INTO COLOR (NAME) VALUES ("+W.quote(name)+");";
    W.exec(sql);
    W.commit();
}


void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            )
{
    bool flag = false; //
    stringstream ss;
    if (use_mpg || use_ppg || use_rpg || use_apg || use_spg || use_bpg){
        ss << "Select * FROM PLAYER WHERE";
    }else{ //all not used
        ss << "Select * FROM PLAYER";
    }

    vector<int> use{use_mpg,use_ppg,use_rpg,use_apg,use_spg,use_bpg};
    vector<int> mins1{min_mpg,min_ppg,min_rpg,min_apg};
    vector<int> maxs1{max_mpg,max_ppg,max_rpg,max_apg};
    vector<double> mins2{min_spg,min_bpg};
    vector<double> maxs2{max_spg,max_bpg};
    vector<string> pgs{"MPG","PPG","RPG","APG","SPG","BPG"};

    for (int i=0;i<use.size();i++){
        if (use[i]!=0){
            if (flag){
                ss << " AND ";
            }else{
                flag = true; //no need to add AND
            }
            if (i<4){
                ss << "(" << pgs[i] << " BETWEEN "<<mins1[i]<<" AND "<<maxs1[i]<<")";
            }else{
                ss << "(" << pgs[i] << " BETWEEN " << mins2[i-4] <<" AND " << maxs2[i-4] <<")";
            }
        }
    }

    ss << ";";
    nontransaction N(*C);
    result R(N.exec(ss.str()));
    cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG"<<endl;
    for (result::const_iterator i = R.begin(); i!=R.end();++i){
        cout << i[0].as<int>() << " " << i[1].as<int>() << " " << i[2].as<int>() << " "<<i[3].as<string>() <<" "
        <<i[4].as<string>() <<" "<<i[5].as<int>() <<" "<<i[6].as<int>()<<" "<<i[7].as<int>()<<" "
        <<i[8].as<int>()<<" "<< fixed << setprecision(1) << i[9].as<double>()<<" "<< i[10].as<double>() <<endl;
    }

}
    


void query2(connection *C, string team_color)
{
    work W(*C);
    stringstream ss;
    ss << "SELECT TEAM.NAME FROM TEAM, COLOR ";
    ss << "WHERE TEAM.COLOR_ID=COLOR.COLOR_ID AND COLOR.NAME="<< W.quote(team_color) << ";";
    W.commit();
    nontransaction N(*C);
    result R(N.exec(ss.str()));
    cout << "NAME" <<endl;
    for (result::const_iterator i = R.begin();i!=R.end();++i){
        cout << i[0].as<string>()<<endl;
    }
}


void query3(connection *C, string team_name)
{
    work W(*C);
    stringstream ss;
    ss << "SELECT FIRST_NAME, LAST_NAME FROM PLAYER, TEAM WHERE TEAM.TEAM_ID=PLAYER.TEAM_ID AND TEAM.NAME=" 
    << W.quote(team_name) <<" ORDER BY PPG DESC;";
    W.commit();
    nontransaction N(*C);
    result R(N.exec(ss.str()));
    cout << "FIRST_NAME LAST_NAME"<<endl;
    for (result::const_iterator i=R.begin();i!=R.end();++i){
        cout << i[0].as<string>() << " "<< i[1].as<string>() << endl;
    }
}


void query4(connection *C, string team_state, string team_color)
{
    work W(*C);
    stringstream ss;
    ss << "SELECT UNIFORM_NUM, FIRST_NAME, LAST_NAME FROM PLAYER,TEAM,STATE,COLOR ";
    ss << "WHERE PLAYER.TEAM_ID=TEAM.TEAM_ID AND TEAM.STATE_ID=STATE.STATE_ID AND TEAM.COLOR_ID=COLOR.COLOR_ID AND ";
    ss << "STATE.NAME=" << W.quote(team_state) << " AND COLOR.NAME=" << W.quote(team_color) << ";";
    W.commit();
    nontransaction N(*C);
    result R(N.exec(ss.str()));
    cout << "UNIFORM_NUM FIRST_NAME LAST_NAME"<<endl;
    for (result::const_iterator i=R.begin();i!=R.end();++i){
        cout << i[0].as<int>() << " " <<i[1].as<string>() << " "<<i[2].as<string>() << endl;
    }
}


void query5(connection *C, int num_wins)
{
    stringstream ss;
    ss << "SELECT FIRST_NAME, LAST_NAME, TEAM.NAME, WINS FROM PLAYER,TEAM ";
    ss << "WHERE PLAYER.TEAM_ID=TEAM.TEAM_ID AND TEAM.WINS>" << num_wins <<";";
    nontransaction N(*C);
    result R(N.exec(ss.str()));
    cout << "FIRST_NAME LAST_NAME NAME WINS" <<endl;
    for (result::const_iterator i=R.begin();i!=R.end();++i){
        cout << i[0].as<string>() << " " <<i[1].as<string>() << " "<<i[2].as<string>() <<" "<<i[3].as<int>()<<endl;
    }
}
