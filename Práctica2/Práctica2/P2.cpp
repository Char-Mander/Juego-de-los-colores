#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <conio.h>
#include <cctype>
#include <fstream>

using namespace std; 




const int MAX_JUGADORES = 10; // Núemero máximo de jugadores que contiene el array
const int MAX_COLORES = 4; // Número máximo de colores que puede contener una secuencia en el modo sencillo.
const int MAX_SECUENCIAS = 11; // Número máximo de secuencias a generar en una partida en el modo sencillo.
const int MAX_COLORES_DIFICIL = 17; //Número máximo de colores que puede contener una secuencia en el modo difícil.
const int MAX_SECUENCIAS_DIFICIL = 15; // Número máximo de secuencias a generar en una partida en el modo difícil.




enum tColores { rojo, azul, dorado, verde, blanco, marrón, naranja}; // Enumerado que contiene los colores
typedef tColores tSecuencia[MAX_COLORES_DIFICIL]; // Array donde está almacenada la secuencia de colores 
enum tModo {fácil, difícil}; // Enumerado en el que se encuentran los dos modos de juego 

typedef struct {
	string nombre;
	float puntuacion;
}tJugadores;

typedef tJugadores tArray[MAX_JUGADORES];

void cargarRanking(tArray, int&); // Función que abre el archivo en el que se encuentra el ranking e introduce los datos en el array topJugadores

int menu(); // Muestra el menú por pantalla.

tModo opcionModo(int); // Cambia modo a fácil o difícil, según la elección de teclado del jugador.

float juego(string, tModo, int); // Función que lleva el control del juego, y devuelve la puntuación. 
					//-Controla la secuencia en la que se encuentra el juego
					//-Lee de teclado los colores que teclea el usuario
					//-Saca por pantalla los correspondientes mensajes (acierto o fallo) 

void generarSecuencia(tSecuencia, int, tModo);  //Procedimiento que genera una secuencia 
// de numColores, que se almacenará en el array secuenciaColores //Función que 

 tColores intToColor(int); //Función que recibe como parámetro un número entero y devuelve un color.
 //El número recibido representa la posición del color en el tipo enumerado.

 tColores charToColor(char); //Función que recibe como parámetro un char introducido de teclado y devuelve su enumerado

 void mostrarSecuencia(const tSecuencia, int); // Muestra la secuencia por pantalla para que el usuario la memorice.

 bool usuarioIntenta(const tSecuencia, int, char, tModo, int&, bool&, float&); // Función que recibe el carácter escrito de teclado, comprueba que
										// sean válidos y le asigna un enumerado a cada carácter.
										
 bool comprobarColor(const tSecuencia, int, tColores, float&, tModo);// Comprueba si el color introducido por el usuario es correcto o no.

 void fallarSecuencia(); // Función que muestra por pantalla un mensaje cuando el usuario falla

 void ganarJuego(); // Función que muestra por pantalla un mensaje cuando el usuario acierta 
					// todos los colores de todas las secuencias

 void utilizarAyuda(const tSecuencia, int, int&, bool&); // Función que comprueba si al usuario le quedan ayudas
								
 void visualizarRanking(tArray); // Función que muestra por pantalla el ranking de los 10 mejores jugadores

 void visualizarMejorJugador(tArray); // Función que muestra por pantalla al jugador con la puntuación más alta

 void actualizarRanking(tArray&, float, string, int&); // Función que se encarga de ordenar los jugadores en el array de tipo estructurado

 void escribirRanking(tArray, int); // Su función es grabar en el fichero top.txt nuevos mejores jugadores que se encuentren en el array de tipo estructurado







int main(){
	locale::global(locale("spanish")); // Incluye tildes, exclamaciones, interrogaciones, ñ...
	string nombre, enter; // Nombre de usuario y tecla 'Enter'
	int o;  // Se introduce de teclado para elegir una opción del menú
	int ayudas = 3; // Variable que corresponde al número de ayudas que tiene el usuario inicialmente
	tModo modo; // Variable de tipo modo, al que se le asignará difícil o fácil, según la opción elegida.
	ifstream archivo; // se declara para leer el archivo
	tArray topJugadores; //variable de tipo tJugadores
	float puntuación; // se le asignará el valor de los puntos que devuelve la función juego
	int ordenTop = 1;  //Número que señalará la cantidad de jugadores que hay en el top.

	srand(time(NULL));

	cargarRanking(topJugadores, ordenTop);


	cout << '\t' << '\t' << '\t' << "/////////////////////////////" << endl;
	cout << '\t' << '\t' << '\t' << "//¡Bienvenido a Simon dice!//" << endl;
	cout << '\t' << '\t' << '\t' << "/////////////////////////////" << '\n' << endl;

	
	cout << "¿Cómo te llamas? ";
	getline(cin, nombre);

	o = menu();
	
		if (o != 0){
			switch (o){
			case 1:
			case 2: modo = opcionModo(o); break;
			case 3: visualizarRanking(topJugadores);
				cout << '\n';
				getline(cin, enter);
				 break;
			case 4:  visualizarMejorJugador(topJugadores);
				cout << '\n';
				getline(cin, enter);
				 break;
			}

			
			while (o >= 3 && o<= 4){
				cout << '\n' << '\n';
				o = menu();
				switch (o){
				case 1:
				case 2: modo = opcionModo(o); break;
				case 3: visualizarRanking(topJugadores);
					cout << '\n';
					getline(cin, enter);
					break;
				case 4:  visualizarMejorJugador(topJugadores);
					cout << '\n';
					getline(cin, enter);
					break;
				}
			}
		}

		else { return 0; }
	


	cout << '\n' <<  nombre << ", pulsa la tecla 'Enter' para empezar a jugar." << endl;
	getline(cin, enter);
	

	puntuación=juego(nombre, modo, ayudas);

	cout << '\n' << '\t' << '\t' << "Ha conseguido un total de " << puntuación << " puntos. " << '\n'<< '\n';

	actualizarRanking(topJugadores, puntuación, nombre, ordenTop);

	escribirRanking(topJugadores, ordenTop);

	system("pause");

	return 0;


}


void cargarRanking(tArray topJugadores, int& ordenTop){
	
	ifstream archivo;
	bool encontradoCentinela;
	const string centinela = "-1"; // centinela del archivo. Cuando lo encuentre debe salirse de él.
	int posicion = 0; // posición del tipo estructurado
	string n;


	archivo.open("top.txt");

	
	if (archivo.is_open()){

		getline(archivo, n);
		
		encontradoCentinela = n == centinela;

			while (!encontradoCentinela && ordenTop < MAX_JUGADORES){
				ordenTop++;
				getline(archivo, n);
			}
			archivo.close();

			archivo.open("top.txt");
			do {
				archivo >> topJugadores[posicion].puntuacion;
				getline(archivo, topJugadores[posicion].nombre);
				cin.sync();

				posicion++;
				

			} while (posicion < ordenTop && !encontradoCentinela);

			archivo.close();
			
		}
	
		

	else {
		cout << '\n' << "Archivo no encontrado..." << '\n' << '\n';
	}


	return;
}



void actualizarRanking(tArray& topJugadores, float puntos, string nombre, int& ordenTop){

	int i = 0, posición = 0, j = ordenTop + 1;
	float min_puntos = topJugadores[0].puntuacion;

	while (i < ordenTop) {
		if (topJugadores[i].puntuacion < min_puntos) {
			min_puntos = topJugadores[i].puntuacion;
			posición = i;
		}
		i++;
	}

	if (puntos >= topJugadores[posición].puntuacion) {
		i = 0;
		while (topJugadores[i].puntuacion > puntos) {
			i++;
		}

		if (ordenTop == MAX_JUGADORES) {
			j = MAX_JUGADORES - 1;
			while (j > i) {
				topJugadores[j].puntuacion = topJugadores[j - 1].puntuacion;
				topJugadores[j].nombre = topJugadores[j - 1].nombre;
				j--;
			}
			topJugadores[j].puntuacion = puntos;
			topJugadores[j].nombre = nombre;
		}

		if (ordenTop < MAX_JUGADORES) {
			j = ordenTop + 1;
			while (j > i) {
				topJugadores[j].puntuacion = topJugadores[j - 1].puntuacion;
				topJugadores[j].nombre = topJugadores[j - 1].nombre;
				j--;
			}
			topJugadores[j].puntuacion = puntos;
			topJugadores[j].nombre = nombre;
			ordenTop++;
		}
	}

	if (puntos < topJugadores[posición].puntuacion && ordenTop < MAX_JUGADORES) {
		topJugadores[ordenTop].puntuacion = puntos;
		topJugadores[ordenTop].nombre = nombre;
		ordenTop++;
	}
}


void escribirRanking(tArray topJugadores, int ordenTop){
	ifstream archivo;
	ofstream archivotop;

	bool encontradoCentinela;
	const string centinela = "-1  ";
	string n;
	int posición = 0;


	archivotop.open("top.txt");


	do {
		archivotop << topJugadores[posición].puntuacion << " " << topJugadores[posición].nombre << '\n';
		posición++;

		getline(archivo, n);

	} while (posición < ordenTop);



	archivotop.close();
}



int menu(){
	int o;

	cout << '\n' << "////////////////////";
	cout << '\n' << "////    MENÚ    ////";
	cout << '\n' << "////////////////////" << '\n';

	cout << '\n' <<  "Elija una opción para continuar: " << endl;
	cout << "0: Salir. " << '\n';
	cout << "1: Jugar en modo sencillo. " << '\n';
	cout << "2: Jugar en modo difícil. " << '\n';
	cout << "3: Ver 10 mejores jugadores. " << '\n';
	cout << "4: Ver jugador(es) con la puntuación más alta. " << '\n';
	cin >> o;

	while (o < 0 || o>4){

		cout << '\n' << "Por favor, introduzca una opción válida. " << '\n';
		cin >> o;
	}

	return o;
}


tModo opcionModo(int o){

tModo  modo;

	switch (o){
	case 1: cout << '\n' << "Has elegido el modo fácil. " << '\n'; return fácil; break;
	case 2: cout << '\n' << "Has elegido el modo difícil. " << '\n'; return difícil; break;
	}

}


float juego(string nombre, tModo modo, int ayudas){
	float puntos=0; // Puntos del usuario. Empiezan con 0 puntos
	string enter; // 'Enter' se pulsará cada vez que se pida para continuar.
	tSecuencia secuenciaColores; // Secuencia que almacena los colores
	int numero = 1; // Número de secuencia
	char color; // Color introducido de teclado por el usuario
	int indice; // Posición de cada enumerado en la secuencia.
	bool acertar = true; // booleando que comprueba si la función comprobarColor devuelve true o false
	int MAXIMO; // Mayor nº de secuencias que se pueden generar y mostrar (independientemente del modo de juego)
	bool vidas; // devuelve true cuando quedan vidas, y false cuando se agotan.
	


	if (modo == fácil){
		MAXIMO = MAX_SECUENCIAS+1;
	}

	else MAXIMO = MAX_SECUENCIAS_DIFICIL+2;
	

	for (int i = 3; i <= MAXIMO; i++) {

		
			
			generarSecuencia(secuenciaColores, i, modo);

			cout << '\n' << "Secuencia número " << (i - 2) << ": " << '\n' << '\n';

			mostrarSecuencia(secuenciaColores, i);

	

		cout << '\n' << '\n' << "Memoriza la secuencia y pulsa 'Enter' para continuar..." << '\n';

		getline(cin, enter);

		system("cls");

		cout << nombre << ", introduzca la secuencia de " << i << " colores: " << '\n';

		for (int j = 0; j < i; j++){

			cin >> color;

			acertar=usuarioIntenta(secuenciaColores, j, color, modo, ayudas, vidas, puntos);
			

			

				if (acertar == false) {

					
					

					fallarSecuencia();

					return puntos;

					system("pause");

					

				}

				else if (acertar== true && j == (i - 1)){

					cout << '\n' << '\t' << '\t' <<  "¡Enhorabuena, acertaste la secuencia número " << numero << "!" << '\n';
					numero = numero + 1;

					switch (modo){
					case fácil:		puntos = puntos + 10; break;
					case difícil:	puntos = puntos + (10 * 1.5); break;
					}
				}
			
		}

		getline(cin, enter);

		} 

	

	ganarJuego();

	return puntos;


	system("pause");

}


void generarSecuencia(tSecuencia secuenciaColores, int numColores, tModo modo){

	
			if (numColores == 3){

				for (int i = 0; i < numColores; i++){

					switch (modo){

						case fácil: secuenciaColores[i] = intToColor(rand() % 4); break;
						case difícil: secuenciaColores[i] = intToColor(rand() % 7); break;
					}
				}
			}
			else
				switch (modo){
					case fácil: secuenciaColores[numColores - 1] = intToColor(rand() % 4); break;
					case difícil: secuenciaColores[numColores - 1] = intToColor(rand() % 7); break;

			}


} 


tColores intToColor(int numero){
	tColores color;

		switch (numero) {

		case 0: color = rojo; break;
		case 1: color = azul; break;
		case 2: color = dorado; break;
		case 3: color = verde; break;
		case 4: color = blanco; break;
		case 5: color = marrón; break;
		case 6: color = naranja; break;
		}

		return color;
}


void mostrarSecuencia(const tSecuencia secuenciaColores, int numero){

	for (int i = 0; i < numero; i++){

	switch (secuenciaColores[i]){

	case rojo: cout << "Rojo "; break;
	case azul: cout << "Azul "; break;
	case dorado: cout << "Dorado "; break;
	case verde: cout << "Verde "; break;
	case blanco: cout << "Blanco "; break;
	case marrón: cout << "Marrón "; break;
	case naranja: cout << "Naranja "; break;


	}
	
	if (i >= 0 && i<(numero-1)) {
		cout << " - ";
	}

	}

}


bool usuarioIntenta(const tSecuencia secuenciaColores, int indice, char color, tModo modo, int& numAyudas, bool& ayudaUsada, float& puntos){
	
	tColores pista; // variable de tipo tColores que se utiliza para mostrar el color que el usuario solicitó saber
	
	
	color = toupper(color); //Transforma todos los caracteres, sean mayúscula o minúscula, a mayúsculas.

	if (color == 'X'){
		utilizarAyuda(secuenciaColores, indice, numAyudas, ayudaUsada);

		switch (ayudaUsada){
		case true:
			cout << '\n' << "El color que has solicitado saber es el ";
			switch (modo){
			case fácil: puntos = puntos - 7; break;
			case difícil: puntos = puntos - (1.5 * 7); break;
			}

			pista = intToColor(secuenciaColores[indice]);

			switch (pista){

			case rojo: cout << "Rojo "; break;
			case azul: cout << "Azul "; break;
			case dorado: cout << "Dorado "; break;
			case verde: cout << "Verde "; break;
			case blanco: cout << "Blanco "; break;
			case marrón: cout << "Marrón "; break;
			case naranja: cout << "Naranja "; break;

			}

			cout << '\n';
			return comprobarColor(secuenciaColores, indice, charToColor(color), puntos, modo) == false; break;
		case false: cout << '\n' << "No te quedan más ayudas. Intenta averiguar el color por ti mismo" << '\n';
			cin >> color;
			color = toupper(color);
			usuarioIntenta(secuenciaColores, indice, color, modo, numAyudas, ayudaUsada, puntos);
			

					return comprobarColor(secuenciaColores, indice, charToColor(color), puntos, modo);

					break;
				
		}
	}
	else {
		color = toupper(color);

		if (modo == fácil){

			while (color != 'R'  && color != 'A'  && color != 'D'  && color != 'V' && color != 'X'){

				cout << '\n' << "Por favor, introduce un carácter válido: ";
				cin >> color;
				color = toupper(color);

			}

		}
		else 

			while (color != 'R'  && color != 'A'  && color != 'D'  && color != 'V' && color != 'B' && color != 'M' && color != 'N'
				&& color != 'X'){

				cout << '\n' << "Por favor, introduce un carácter válido: ";
				cin >> color;
				color = toupper(color);

			}
		
	}

	

		return comprobarColor(secuenciaColores, indice, charToColor(color), puntos, modo);
}


bool comprobarColor(const tSecuencia secuenciaColores, int indice, tColores color, float& puntos, tModo modo){
	if (secuenciaColores[indice] == color){
		switch (modo){
		case fácil:	puntos = puntos + 5; break;
		case difícil: puntos = puntos + (5 * 1.5); break;
		}

		return true;


	}
	else {
		return false;
	}
}

void utilizarAyuda(const tSecuencia secuenciaColores, int indice, int &numAyudas, bool& ayudaUsada){

	if (numAyudas > 0){
		numAyudas = numAyudas - 1;
		ayudaUsada =  true;
		
	}
	else
		ayudaUsada = false;
	
}


tColores charToColor(char color){

	switch (color) {
	case 'R': return rojo; 
	case 'A': return azul; 
	case 'D': return dorado;
	case 'V': return verde; 
	case 'B': return blanco;
	case 'M': return marrón;
	case 'N': return naranja;
	}

}


void fallarSecuencia(){

	cout << '\n' << '\t' << '\t' << '\t' << "     ¡Fallaste la secuencia! " << '\n' << endl;

	cout << '\t' << '\t' << '\t' << " ******************************* " << '\n';
	cout << '\t' << '\t' << '\t' << " ******************************* " << '\n';
	cout << '\t' << '\t' << '\t' << " ******                   ****** " << '\n';
	cout << '\t' << '\t' << '\t' << " ******     GAME OVER     ****** " << '\n';
	cout << '\t' << '\t' << '\t' << " ******                   ****** " << '\n';
	cout << '\t' << '\t' << '\t' << " ******************************* " << '\n';
	cout << '\t' << '\t' << '\t' << " ******************************* " << '\n' << '\n' << '\n' << '\n';

}


void ganarJuego(){

	cout << '\n' <<'\n' << '\t' << '\t' << '\t' << '\t' << "  ¡Ganaste!" << '\n' << '\n';

	cout << '\t' << '\t' << '\t' << "       **           **      " << '\n';
	cout << '\t' << '\t' << '\t' << "       **           **      " << '\n';
	cout << '\t' << '\t' << '\t' << "         **       **        " << '\n';
	cout << '\t' << '\t' << '\t' << "         **       **        " << '\n';
	cout << '\t' << '\t' << '\t' << "       ***************      " << '\n';
	cout << '\t' << '\t' << '\t' << "       ***************      " << '\n';
	cout << '\t' << '\t' << '\t' << "     ****  ******  ****     " << '\n';
	cout << '\t' << '\t' << '\t' << "     ****  ******  ****     " << '\n';
	cout << '\t' << '\t' << '\t' << "   **********************   " << '\n';
	cout << '\t' << '\t' << '\t' << "   **********************   " << '\n';
	cout << '\t' << '\t' << '\t' << "   **  **************  **   " << '\n';
	cout << '\t' << '\t' << '\t' << "   **  **************  **   " << '\n';
	cout << '\t' << '\t' << '\t' << "   **  **          **  **   " << '\n';
	cout << '\t' << '\t' << '\t' << "   **  **          **  **   " << '\n';
	cout << '\t' << '\t' << '\t' << "         ****  ****         " << '\n';
	cout << '\t' << '\t' << '\t' << "         ****  ****         " << '\n' << '\n' << '\n';

}


void visualizarRanking(tArray topJugadores){

	cout << '\n' << "//////////////////////////////////////";
	cout << '\n' << "////                              ////";
	cout << '\n' << "////   TOP 10 MEJORES JUGADORES   ////";
	cout << '\n' << "////                              ////";
	cout << '\n' << "//////////////////////////////////////" << '\n';
	for (int i = 0; i < MAX_JUGADORES; i++){

		cout << '\n' << " Jugador número " << (i + 1) << ": ";
		cout << topJugadores[i].nombre << " con una puntuación de: " << topJugadores[i].puntuacion << '\n';

	}
}


void visualizarMejorJugador(tArray topJugadores){
	string mejorJugador;
	bool encontrado = false;
	int i = 0, j = 0;
	
	float mayorPuntuacion = 0;

	do {
		if (mayorPuntuacion<topJugadores[i].puntuacion){

			mayorPuntuacion = topJugadores[i].puntuacion;
			j = i;
		}



		i++;

	} while (i < MAX_JUGADORES);

	cout << '\n' << "El mejor jugador es " << topJugadores[j].nombre << '\n';


}
