#include "testApp.h"
#include <ctime>
#include <time.h>
using namespace sql;



 testApp::testApp(): 
	driver(0)
	, con(0)
	, smt(0)
	, res(0)
	,prep_smt(0)
 {


	/*	SQLString host("localhost");
	SQLString usser("root");
	SQLString pass("edis");
	driver = get_driver_instance();
	con = driver->connect(host, usser, pass);
	cout <<"todo ok"<<endl;
*/
}
void testApp::setup(){
	
		string conex ;
		cout << "INGRESA EL HOST A CONECTAR:  "; 
		
		cin >> host;
		cout<<endl;
		cout << "INGRESA EL NOMBRE DE USUARIO: "; 
		
		cin >> usser;
		cout<<endl;
		cout << "INGRESA EL PASWORD:  "; 
		 
		cin >> pass;
		cout<<endl;


	try {
		
		driver =  get_driver_instance(); 
		cerr << "apunto de carash"<< endl;
		con = driver->connect( host.c_str(),usser.c_str() , pass.c_str());
		cerr << "paso el crash"<< endl;
		
		con->setSchema("test");
		cout << smt << endl;
	}
	catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}

	int timeCount = 0;
	ofBackground(0,0,0);
	ofSetFullscreen(true);
 	ofEnableAlphaBlending();
	spray.loadImage("spray.png");
	// ofSetFrameRate(60);
	cout << "***********INGRESAR DATOS DE ARRANQUE*************" << endl << endl;
	string puerto;
	cout << "INGRESAR PUERTO SERIAL DONDE ESTA CONECTADA LA LATA:   ";
	cin >> puerto;
	lata.setupPuerto("COM"+puerto,9600);
	cout<< "INGRESAR NUMERO DE PIZARRA:   ";
	cin >> pizarraID;
	
	cerr << " arrancando..."<< endl;
	
	
//EXTRACCIÓN DE LINEAS DEL SERVIDOR INCLUYENDO LAS LINEAS REALIZADAS PREVIAMENTE POR ESTA PIZATRRA 	
	prep_smt = con->prepareStatement("SELECT ID, LIN, COL, LENGTH(LIN),PIZARRA,TIME, DATE_SUB(NOW(),INTERVAL 1 MINUTE)  FROM tests WHERE TIME > DATE_SUB(NOW(),INTERVAL 1 DAY) AND PIZARRA = ?");
	prep_smt->setString(1,pizarraID.c_str());
	res = prep_smt->executeQuery();
	
	while(res->next()){

			vector<int> x;
			vector<int> y;
			int _r,_g,_b;
			Trazo SQLtz;
			ofVec2f SQLvec2;
			vector<ofVec2f> SQLlinVec;
			
			
			
			int id = res->getInt(1);
		
			string sLin = res->getString(2).c_str();/// ESTAS LINEAS GENERAN ERROR EN EL RELEASE
			string rgb = res->getString(3).c_str();/// ESTAS LINEAS GENERAN ERROR EN EL RELEASE
		
			int largosLin = int(res->getInt(4));
			int pizarra = int(res->getInt(5));
			string time = res->getString(6).c_str();
			string hacetiempo = res->getString(7).c_str();

			
			while(checarSiIDexist(id) && largosLin > 1){
				
				cout << "****************linea nueva****************"<< endl;
				cout << "ID: "<< id << " | color: " <<rgb << " | de la pizarra:  "<< pizarra << " hecha en: "<< time << endl;	
					stringstoints(sLin,largosLin, &x, &y);
			
					if((x.size()== y.size())&& (x.size() >= 1))	{

			
						for(int h = 0; h < x.size(); h++){
				
							SQLvec2.set(x.at(h),y.at(h));
							SQLlinVec.push_back(SQLvec2);
						}
			
						SQLtz.setVtrazo(SQLlinVec);
				
					}else{
						cout << "puntos X y Y no coinciden" << endl;
		
						}
					RGBstringtoRGBint(rgb, &_r,&_g,&_b);

					SQLtz.setColor(_r,_g,_b);
					SQLidsON.push_back(id);
				lin.push_back(SQLtz);
			cout << "**************linea fin*********************"<< endl;
			}
			
		
	}
//FIN DE LA EXTRACCIÓN

}

//--------------------------------------------------------------
void testApp::update(){
	
    lata.updateColor();

	/*
		
	if (timeCount % 1000 = 0){
	

	
	
	}
	*/

//LOOP DE BUSQUEDA DE NUEVOS TRAZOS EN SERVIDOR
	t = clock();
	if( (t % 5000) < 16){
	 	prep_smt = con->prepareStatement("SELECT ID, LIN, COL, LENGTH(LIN),PIZARRA,TIME, DATE_SUB(NOW(),INTERVAL 1 MINUTE)  FROM tests WHERE TIME > DATE_SUB(NOW(),INTERVAL 1 DAY) AND PIZARRA != ?");
		prep_smt->setString(1,pizarraID.c_str());
		res = prep_smt->executeQuery();
		
			while(res->next()){

			
				vector<int> x;
				vector<int> y;
				int _r,_g,_b;
				Trazo SQLtz;
				ofVec2f SQLvec2;
				vector<ofVec2f> SQLlinVec;
			
			
			
				int id = int(res->getInt(1));
				string sLin =  res->getString(2).c_str();/// ESTAS LINEAS GENERAN ERROR EN EL RELEASE
				string rgb = res->getString(3).c_str();/// ESTAS LINEAS GENERAN ERROR EN EL RELEASE
				int largosLin = int(res->getInt(4));
				int pizarra = int(res->getInt(5));
				string time = res->getString(6).c_str();
				string hacetiempo = res->getString(7).c_str();

			
				while(checarSiIDexist(id) && largosLin > 1 ){
					cout << "****************linea nueva****************"<< endl;
				cout << "ID: "<< id << " | color: " <<rgb << " | de la pizarra:  "<< pizarra << " hecha en: "<< time << endl;	
					stringstoints(sLin,largosLin, &x, &y);
					if((x.size()== y.size()) && (x.size() >= 1))	{
						for(int h = 0; h < x.size(); h++){
							SQLvec2.set(x.at(h),y.at(h));
							SQLlinVec.push_back(SQLvec2);
						}
						SQLtz.setVtrazo(SQLlinVec);
					}else{
						cout << "puntos X y Y no coinciden" << endl;
						}
					RGBstringtoRGBint(rgb, &_r,&_g,&_b);
					SQLtz.setColor(_r,_g,_b);
					SQLidsON.push_back(id);
					lin.push_back(SQLtz);
				cout << "**************linea fin*********************"<< endl;
				}

		}
	
	}
// FIN LOOP

}

//--------------------------------------------------------------
void testApp::draw(){
	ofEnableSmoothing();
	//----------------------------------------------------------
	/*
	SE DIBUJA CADA TRAZO GUARDADO EN EL VECTOR ESTATICO
	*/
		if( !lin.empty() ){
		  //  cout << "lineas on"<< endl;
		   for(int l= 0; l< lin.size(); l++){
				for(int p = 1; p < lin.at(l).getTrazoSize(); p++){
				  ofSetColor(lin.at(l).getColor());
				  spray.draw((floor(lin.at(l).getVtrazo(p).x)-25),(floor(lin.at(l).getVtrazo(p).y))-25);
				}
			}
		}
	//------------------------------------------------------------
	/*
	SE DIBUJA EL VECTOR TEMPORAL PARA QUE SE VEA LO QUE SE DIBUJA AUN MIENTRAS SE SIGUEN AUMENTANDO
	PUNTOS A ESTE VECTOR TEMPORAL
	*/
		 if( !temp.empty() ){
			for(int p = 1; p < temp.size(); p++){
				if(temp.size() >= 2){
					ofSetColor(colTemp);
					spray.draw((floor(temp.at(p).x)-25),(floor(temp.at(p).y))-25);
				}
			}
		}
}
//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == 'c'){
    lin.clear();
    }
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
 //   cout <<"drag on"<<endl;
            ofVec2f pts;
            pts.set(x,y);
            temp.push_back(pts);

       //     cout << "cantidad de puntos = "<< temp.size()<<endl;
}
//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    cout << "mouse oprimido" << endl;
    colTemp = lata.getRGBcolor(&colTempString);
}
//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    cout << "mouse levantado"<< endl;
	if( !temp.empty() ){
        for(int p = 1; p < temp.size(); p++){
			stringstream sx;
			sx << temp.at(p).x;
			stringstream sy;
			sy << temp.at(p).y;

			bLinTemp.append( ":");
			bLinTemp.append( sx.str());
			bLinTemp.append( ":");
			bLinTemp.append(sy.str());
			bLinTemp.append( ":");

        }

		try{
		prep_smt = con->prepareStatement("INSERT INTO tests(LIN,COL,PIZARRA) VALUES(?,?,?)");		
		prep_smt->setString(1,bLinTemp.c_str());
		bLinTemp.clear();
		prep_smt->setString(2,colTempString.c_str());
		colTempString.clear();
		int idpizarraint = atoi(pizarraID.c_str());
		prep_smt->setInt(3,idpizarraint);
		prep_smt->execute();		

		cout << "Se guardaro "<< endl;
		
		}catch(sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}
//	
//	//se envia bLinTemp a servidor con colorTemp
//
    Trazo tz;
    tz.setVtrazo(temp);
    tz.setColor(colTemp);
    lin.push_back(tz);
    temp.clear();
    cout << "Vector de de puntos vaciado"<< endl;
}

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){


}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}


void testApp::stringstoints(string SQLlin, int largoSQLlin,  vector<int> *vx, vector<int> *vy){
	/*ESTA FUNCION REGRESA CONVIERTE UNA CADENA DE CARACTERES A VECTORES X Y Y*/
		int marcador = 0;
		int prevMark = 0;	
		
		string match = ":";
		string matched;
		matched = SQLlin.substr(SQLlin.length()-1,1);

		if(match.compare(matched) == 0){
			
			while(marcador < largoSQLlin){
						
				string sx;
				string sy;
				int ix;
				int iy;
					

				marcador = SQLlin.find(":",marcador);
				prevMark = int(marcador+1);
				marcador = SQLlin.find(":",marcador+1);
				sx = SQLlin.substr(prevMark,int(marcador)-prevMark);
				ix = atoi(sx.c_str());

				vx->push_back(ix);

				marcador = SQLlin.find(":",marcador);
				prevMark = int(marcador+1);
				marcador = SQLlin.find(":",marcador+1);
				sy = SQLlin.substr(prevMark,int(marcador)-prevMark);

					

				iy = atoi(sy.c_str());

				vy->push_back(iy);
					

				marcador++;
		}
	}
}

void testApp::RGBstringtoRGBint(string rgb, int *_r, int *_g, int *_b){
									
		int marcador = 0;
		int prevMark = 0;

		string rs,gs,bs;
		string match = ":";
		string matched;
		matched = rgb.substr(rgb.length()-1,1);

		if(match.compare(matched) == 0 && rgb.size() < 16){

			while(marcador < rgb.rfind(":")){	
				marcador = rgb.find(":",marcador);
				prevMark = int(marcador+1);
				marcador = rgb.find(":",marcador+1);
				rs = rgb.substr(prevMark,int(marcador)-prevMark);
				marcador = rgb.find(":",marcador);
				prevMark = int(marcador+1);
				marcador = rgb.find(":",marcador+1);
				gs = rgb.substr(prevMark,int(marcador)-prevMark);
				marcador = rgb.find(":",marcador);
				prevMark = int(marcador+1);
				marcador = rgb.find(":",marcador+1);
				bs = rgb.substr(prevMark,int(marcador)-prevMark);
				marcador++;
				int ri = atoi(rs.c_str());
				int gi = atoi(gs.c_str());
				int bi = atoi(bs.c_str());

				cout << "color a pintar R:"<< ri <<" | "<<"G:"<<gi<<" | "<< "B:"<<bi<< endl;
				*_r = ri;
				*_g = gi;
				*_b = bi;
		}
	}

}	


bool testApp::checarSiIDexist(int _id){

	bool resb;
	if(SQLidsON.size() >= 1){	
			cout << "Cantidad de SQLs IDs: "<< SQLidsON.size()<< endl;
			for(int f= 0; f < SQLidsON.size(); f++){
				if (SQLidsON.at(f) == _id){

				resb = false;
				 return resb;
				 cout<<"linea existente" <<endl;
				}
			}
			cout<<"no hay puntos asi en el programa" <<endl<< "guardando..."<<endl;
			resb = true;
			return resb;
	}

	resb = true;
	return resb;
}

	