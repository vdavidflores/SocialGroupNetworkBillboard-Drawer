#pragma once
#include <vector>
#include <string>
#include <cassert>
#include <stdexcept>
#include <limits>


#include <cppconn\driver.h>
#include <cppconn\connection.h>
#include <cppconn\statement.h>
#include <cppconn\prepared_statement.h>
#include <cppconn\resultset.h>
#include <cppconn\exception.h>
#include <cppconn\sqlstring.h>


#include "ofMain.h"
using namespace std;

class Trazo{
    public:
        Trazo(){
        }
        void setColor(ofColor color_){

               color = color_;
        }
		void setColor(int _r,int _g,int _b){

			color.r= _r;
			color.g = _g;
			color.b = _b;
        }
        ofColor getColor( ){
            return color;

        }
        int getTrazoSize(){
            return linea.size();
        }
        void setVtrazo(vector <ofVec2f> vec){
            linea = vec;

        }
        ofVec2f getVtrazo(int pt){
            return linea.at(pt);
        }
        void clear(){
        linea.clear();

        }
    protected:
        std::vector <ofVec2f> linea;
        ofColor color;

};


//=============================================================================
class serialCom{

    private:

//................................................................................
    int R,G,B;
    ofSerial puerto;
    int iVal;
    unsigned char bufColor[1];

//................................................................................

    float map(float x, float in_min, float in_max, float out_min, float out_max)
        {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
        }

//.................................................................................
    void colorPuerto(int VAP){
        if(VAP <= 0 ){
        R = 255;
        G = 255;
        B = 255;

    }else if(VAP<330 && VAP >=1 ){
        R = map(VAP,1,330,255,0);
        G = map(VAP,1,330,0,255);
        B = 0;
    }else if(VAP<=660 && VAP >=1){
        R = 0;
        G = map(VAP,330,660,255,0);
        B = map(VAP,330,660,0,255);

    }else if(VAP<=990 && VAP >=1){
        R = map(VAP,661,990,0,255);
        G = 0;
        B = map(VAP,661,990,255,0);
    }else{
        R = 0;
        G = 0;
        B = 0;
        }
    }
//................................................................
public:

//...........................................................................
    void updateColor(){
        puerto.readBytes(bufColor, 1);
    }
//............................................................................
    void setupPuerto(std::string com,int rate){
      
        puerto.setup(com,rate);
     
    }
//............................................................................
    ofColor getRGBcolor()
    {
        iVal = bufColor[0];
        iVal= iVal*32;
        cout<< iVal<<endl;
        colorPuerto(iVal);
        return ofColor(R,G,B,255);
    }
	ofColor getRGBcolor(string *s)
    {
        iVal = bufColor[0];
        iVal= iVal*32;
        cout<< iVal<<endl;
        colorPuerto(iVal);
		char *r= new char(1);
		char *g= new char(1);
		char *b= new char(1);
		itoa(R,r,10);itoa(G,g,10);itoa(B,b,10);
		s->append(":");
		s->append(r);
		s->append(":");
		s->append(g);
		s->append(":");
		s->append(b);
		s->append(":");
		/*delete r;
		delete g;
		delete b;*/
        return ofColor(R,G,B,255);
    }
};

//====================================================================================

class testApp : public ofBaseApp{
	public:

		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *smt;
		sql::ResultSet *res;
		sql::PreparedStatement *prep_smt;
		
		testApp();
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void stringstoints(string SQLlin, int largoSQLlin, vector<int> *vx,vector<int> *vy);
		void RGBstringtoRGBint(string rgb, int *_r, int *g, int *b);
		bool checarSiIDexist(int _id);

		
		//vector<ofVec2f> nLn;
		string pizarraID; // ID de la pizarra en uso
		//basic_string<char[10]> pizzara_id;
        std::vector<Trazo> lin;
        std::vector<ofVec2f> temp;
		std::vector<ofVec2f> sqltemp;
		string bLinTemp;// linea temporal en binario para mandar a sevidor
        ofColor colTemp;
		string colTempString;
		ofImage spray;
		serialCom lata;
		time_t t;
		vector<int> SQLidsON;
		

			string host ;
			string usser; 
		string pass;
		
};


