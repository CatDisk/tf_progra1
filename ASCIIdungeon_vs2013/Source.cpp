/*
Trabajo final para programacion 1, seccion SI1A.

Profesor:
	-Victor Parasi

Autores:
	-Agustin Mansilla
	-Dominic Schialer
*/


#include <conio.h>
#include <stdio.h>
#include <iostream>

using namespace System;
using namespace std;


#define KEY_UP 72 // estos son los scancodes de las teclas direccionales, las uso en la funcion Captura()
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

// estas son las declaraciones de las funciones. es bueno para ver todas de un vistazo.
void Dibujante(int *menu_state, int *hp_jugador, int *cant_pociones, int *dmg, bool *tiene_llave, bool *uso_pocion, int *id_enemigo, int *hp_enemigo, int **matriz, int *fila, int *columna);
//dibuja la pantalla del juego, llama las funciones Estado(), Texto() e ImprimirMatriz()
void Estado(int i, int j, int *hp_jugador, int *cant_pociones, int *dmg, bool *tiene_llave, bool *uso_pocion);
// dibuja el estado del jugador (HP, pociones, etc)
void Texto(int i, int j, int *id_enemigo, int *hp_enemigo);
// dibuja el texto (lo que sucede cada turno), llama la funcion ImprimirEnemigo()
void MenuPrincipal(int *menu_state, int *game_screen);
// dibuja el menu principal
void Captura(int *menu, bool *es_enter, int *scancode, bool *es_esc, int *status, bool *uso_pocion, int *id_enemigo);
// captura las teclas presionadas
void PantallaAyuda(int *game_screen);
// dibuja el menu de ayuda
void ImprimirEnemigo(int *id_enemigo, int *hp_enemigo);
// imprime el tipo de enemigo y el hp, segun el valor de *id_enemigo
void GenerarMatriz(int **matriz, int *fila, int *columna);
// le asigna valores a una matriz
int** CrearMatriz(int *fila, int *columna);
// crea una matriz del tamano *fila x *columna
void ImprimirMatriz(int **matriz, int *fila, int *columna);
// imprime la matriz **matriz
int** GenerarPieza(int *id_pieza);
// crea y genera una matriz de 3 x 3 segun *id_pieza
void GenerarMapa(int **matriz, int *fila, int *columna);
// genera una matriz con los valores *id_pieza (las dimensiones de la matriz dada tienen que ser de (*fila / 3) x (*columna / 3)
void RotarPieza(int **pieza, int *dir);
/* rota la pieza
*dir | grados
0      0
1     90
2    180
3    270    */

// aqui comienzan las funciones
void Dibujante(int *menu_state, int *hp_jugador, int *cant_pociones, int *dmg, bool *tiene_llave, bool *uso_pocion, int *id_enemigo, int *hp_enemigo, int **matriz, int *fila, int *columna)
{
	char lo = 201;
	char hori = 205;
	char ro = 187;
	char verti = 186;
	char ru = 188;
	char lu = 200;
	char lT = 204;
	char rT = 185;
	char space = 255;

	Console::Clear();

	ImprimirMatriz(matriz, fila, columna);

	for (int j = 0; j < 24; j++) // fila de 1 a 24
	{
		for (int i = 0; i < 29; i++) // HUD
		{
			Console::SetCursorPosition(i + 51, j);

			if (j == 0) // linea superior
			{
				if (i == 0)
					cout << lo;
				else if (i == 28)
					cout << ro;
				else
					cout << hori;
			}
			else if (j == 10) // linea del medio
			{
				if (i == 0)
					cout << lT;
				else if (i == 28)
					cout << rT;
				else
					cout << hori;
			}
			else if (j == 23) // linea inferior
			{
				if (i == 0)
					cout << lu;
				else if (i == 28)
					cout << ru;
				else
					cout << hori;
			}
			else if (i == 0 || i == 28)
				cout << verti;

			if (j > 0 && j < 10)
				Estado(i, j, hp_jugador, cant_pociones, dmg, tiene_llave, uso_pocion);

			if (j > 10 && j < 23)
				Texto(i, j, id_enemigo, hp_enemigo);
		}
	}
	*menu_state = 1;
}

void Estado(int i, int j, int *hp_jugador, int *cant_pociones, int *dmg, bool *tiene_llave, bool *uso_pocion)  // Estado del jugador
{
	char verti = 186;
	char space = 255;
	char block = 219;
	char points = 176;

	if (*uso_pocion)
	{
		if (*hp_jugador < 5 && *cant_pociones > 0)
		{
			*hp_jugador += 1;
			*cant_pociones -= 1;
		}
		*uso_pocion = false;
	}

	Console::SetCursorPosition(i + 51, j);

	if (i == 2 && j == 2) // 2da linea: HP
	{
		cout << "RAM: " << *hp_jugador << "/5";
	}
	else if (i == 11 && j == 2)
	{
		Console::ForegroundColor = ConsoleColor::DarkGray;
		for (int z = 0; z < 15 - (*hp_jugador * 3); z++)
			cout << points;
		Console::ForegroundColor = ConsoleColor::DarkRed;
		for (int z = 0; z < *hp_jugador * 3; z++)
			cout << block;
		Console::ForegroundColor = ConsoleColor::Gray;
	}
	else if (i == 2 && j == 4) // 4ta linea: ataque
	{
		cout << "Nivel de Corrupcion: +" << *dmg;
	}
	else if (i == 2 && j == 6) // 6ta linea: pociones
	{
		cout << "Pointers: " << *cant_pociones;
	}
	else if (i == 2 && j == 8) // 8va linea: tiene la llave?
	{
		if (*tiene_llave)
		{
			cout << "Tienes la ";
			Console::ForegroundColor = ConsoleColor::Red;
			cout << "CLAVE";
			Console::ForegroundColor = ConsoleColor::Gray;
			cout << "!";
		}

		else
		{
			cout << "No tienes la ";
			Console::ForegroundColor = ConsoleColor::Red;
			cout << "CLAVE";
			Console::ForegroundColor = ConsoleColor::Gray;
		}
	}
}

void Texto(int i, int j, int *id_enemigo, int *hp_enemigo)
{
	int _dmg = 4;

	Console::SetCursorPosition(i + 51, j);

	if (j == 12 && i == 2)
	{
		cout << "Tu encuentras:";
	}
	else if (j == 13 && i == 2)
	{
		ImprimirEnemigo(id_enemigo, hp_enemigo);
		cout << " con " << *hp_enemigo << " HP!";
	}
	else if (j == 15 && i == 2)
	{
		cout << "Inyectas " << _dmg << " bytes!";
	}
	else if (j == 17 && i == 2)
	{
		ImprimirEnemigo(id_enemigo, hp_enemigo);
		cout << " es corrompido.";
	}
	else if (j == 22 && i == 2)
	{
		Console::ForegroundColor = ConsoleColor::DarkGray;
		cout << "Enter para continuar...";
		Console::ForegroundColor = ConsoleColor::Gray;
	}
}

void ImprimirEnemigo(int *id_enemigo, int *hp_enemigo)
{
	switch (*id_enemigo)
	{
	case 0:
		cout << "int";
		*hp_enemigo = 5;
		break;
	case 1:
		cout << "double";
		*hp_enemigo = 6;
		break;
	case 2:
		cout << "string";
		*hp_enemigo = 8;
		break;
	case 3:
		cout << "function()";
		*hp_enemigo = 7;
		break;
	case 4:
		cout << "class";
		*hp_enemigo = 10;
		break;
	case 5:
		cout << "library";
		*hp_enemigo = 13;
		break;
	case 6:
		cout << "cmd.exe";
		*hp_enemigo = 12;
		break;
	case 7:
		cout << "rootmdm.sys";
		*hp_enemigo = 12;
		break;
	case 8:
		cout << "system32";
		*hp_enemigo = 15;
		break;
	}
}

void MenuPrincipal(int *menu_state, int *game_screen)
{
	Console::Clear();

	for (int j = 0; j < 16; j++)
	{
		Console::SetCursorPosition(25, j + 2);
		switch (j)
		{
		case 0:
			cout << " _____ _____ _____ _____ _____ ";
			break;
		case 1:
			cout << "|  |  |     | __  |  |  |   __|";
			break;
		case 2:
			cout << "|  |  |-   -|    -|  |  |__   |";
			break;
		case 3:
			cout << " \\___/|_____|__|__|_____|_____|";
			break;
		case 4:
			Console::ForegroundColor = ConsoleColor::DarkRed;
			cout << "    _                         ";
			break;
		case 5:
			cout << "  _| |_ _ ___ ___ ___ ___ ___ ";
			break;
		case 6:
			cout << " | . | | |   | . | -_| . |   |";
			break;
		case 7:
			cout << " |___|___|_|_|_  |___|___|_|_|";
			break;
		case 8:
			cout << "             |___|            ";
			Console::ForegroundColor = ConsoleColor::Gray;
			break;
		case 11:
			if (*menu_state == 0)
			{
				Console::BackgroundColor = ConsoleColor::DarkRed;
				Console::ForegroundColor = ConsoleColor::Black;
			}
			cout << "[  run hack.bat   ]";
			Console::BackgroundColor = ConsoleColor::Black;
			Console::ForegroundColor = ConsoleColor::Gray;
			break;
		case 13:
			if (*menu_state == 1)
			{
				Console::BackgroundColor = ConsoleColor::DarkRed;
				Console::ForegroundColor = ConsoleColor::Black;
			}
			cout << "[   README.txt    ]";
			Console::BackgroundColor = ConsoleColor::Black;
			Console::ForegroundColor = ConsoleColor::Gray;
			break;
		case 15:
			if (*menu_state == 2)
			{
				Console::BackgroundColor = ConsoleColor::DarkRed;
				Console::ForegroundColor = ConsoleColor::Black;
			}
			cout << "[ system.shutdown ]";
			Console::BackgroundColor = ConsoleColor::Black;
			Console::ForegroundColor = ConsoleColor::Gray;
			break;
		default:
			break;
		}
	}
	*game_screen = 0;
}

void PantallaAyuda(int *game_screen)
{
	char hori = 205;
	char verti = 186;
	char lo = 201;
	char ro = 187;
	char ru = 188;
	char lu = 200;
	char lT = 204;
	char rT = 185;
	char jugador = 254;
	char llave = 'C';
	char tesoro = 'D';
	char salida = 'S';
	char pared = 219;
	char piso = 176;

	Console::Clear();

	for (int j = 0; j < 24; j++) // fila
	{
		for (int i = 0; i < 80; i++) // columna
		{
			Console::SetCursorPosition(i, j);

			if (j == 0)
			{
				if (i == 0)
					cout << lo;
				else if (i == 79)
					cout << ro;
				else
					cout << hori;
			}
			if (j > 0 && j < 15)
			{
				if (i == 0 || i == 79)
					cout << verti;
				else if (i == 4)
				{
					switch (j - 3)
					{
					case 0:
						Console::ForegroundColor = ConsoleColor::Yellow;
						cout << llave;
						break;
					case 2:
						Console::ForegroundColor = ConsoleColor::Magenta;
						cout << tesoro;
						break;
					case 4:
						Console::ForegroundColor = ConsoleColor::Green;
						cout << salida;
						break;
					case 6:
						Console::ForegroundColor = ConsoleColor::DarkRed;
						cout << pared << piso << piso << pared;
						break;
					case 8:
						Console::ForegroundColor = ConsoleColor::Gray;
						cout << pared << piso << piso << pared;
						break;
					case 10:
						Console::ForegroundColor = ConsoleColor::Cyan;
						cout << jugador;
						break;
					default:
						break;
					}
					Console::ForegroundColor = ConsoleColor::Gray;
				}

				else if (i == 17)
				{
					switch (j - 3)
					{
					case 0:
						cout << "Clave de acceso        (nesecitaras esto)";
						break;
					case 2:
						cout << "Directorio             (a.k.a datos desprotegidos)";
						break;
					case 4:
						cout << "Salida del Nivel       (esto todavia sigue!?)";
						break;
					case 6:
						cout << "Memoria NO Explorada   (cuidado, antivirus!)";
						break;
					case 8:
						cout << "Memoria Explorada      (totalmente seguro, confia en mi ...)";
						break;
					case 10:
						cout << "El Jugador             (efectivamente, este eres tu)";
						break;
					default:
						break;
					}
				}
			}
			if (j == 15)
			{
				if (i == 0)
					cout << lT;
				else if (i == 79)
					cout << rT;
				else
					cout << hori;
			}
			if (j > 15 && j < 23)
			{
				if (i == 0 || i == 79)
					cout << verti;
				else if (i == 4)
				{
					Console::ForegroundColor = ConsoleColor::White;
					switch (j - 17)
					{
					case 0:
						cout << "Arrow Keys";
						break;
					case 2:
						cout << 'P';
						break;
					case 4:
						cout << "Esc";
						break;
					}
					Console::ForegroundColor = ConsoleColor::Gray;
				}

				else if (i == 17)
				{
					switch (j - 17)
					{
					case 0:
						cout << "para moverse                 (...duh)";
						break;
					case 2:
						cout << "para acceder a un puntero    (si te quedan algunos)";
						break;
					case 4:
						cout << "para salir al Menu Principal (no te culpo si me dejas (T_T) )";
						break;
					}
				}
			}
			if (j == 23)
			{
				if (i == 0)
					cout << lu;
				else if (i == 79)
					cout << ru;
				else
					cout << hori;
			}

		}
	}
	*game_screen = 2;
}

void Captura(int *menu, bool *enter, int *scancode, bool *esc, int *status, bool *uso_pocion, int *id_enemigo)
{
	switch ((*scancode = _getch()))
	{
	case KEY_UP:
		//cout << endl << "Up" << endl;
		if (*menu != 0 && *status == 0)
			*menu -= 1;
		else if (*status == 0)
			*menu = 2;
		break;
	case KEY_DOWN:
		//cout << endl << "Down" << endl;
		if (*menu != 2 && *status == 0)
			*menu += 1;
		else if (*status == 0)
			*menu = 0;
		break;
	case KEY_LEFT:
		//cout << endl << "Left" << endl;

		break;
	case KEY_RIGHT:
		//cout << endl << "Right" << endl;
		break;
	case 13:
		//cout << endl << "Enter" << endl;
		*enter = true;
		break;
	case 27:
		*esc = true;
		break;
	case 112:
		if (*status == 1)
			*uso_pocion = true;
		break;
	case 97:
		if (*id_enemigo != 8 && *status == 1)
			*id_enemigo += 1;
		else if (*status == 1)
			*id_enemigo = 0;
	default:
		//cout << endl << *scancode << endl;
		break;
	}

	if (*esc && *status == 1 || *status == 2)
	{
		*status = 0;
		*esc = false;
	}
	if (*status == 0)
	{
		if (*enter && *menu == 0)
			*status = 1;

		if (*enter && *menu == 1)
			*status = 2;

		if (*enter && *menu == 2)
			*status = 3;

		*enter = false;
	}
}

int** CrearMatriz(int *fila, int *columna)
{
	int **auxMatrix = new int *[*fila];
	for (int i = 0; i < *fila; i++)
	{
		auxMatrix[i] = new int[*columna];
	}
	return auxMatrix;
}

void GenerarMatriz(int **matriz, int *fila, int *columna)
{
	int counter = 0;
	for (int i = 0; i < *fila; i++)
	{
		for (int j = 0; j < *columna; j++)
		{
			if (counter % 2 == 0)
				matriz[i][j] = 1;
			else
				matriz[i][j] = 2;
			counter++;
		}
		counter++;
	}
}

void ImprimirMatriz(int **matriz, int *fila, int *columna)
{
	for (int i = 0; i < *fila; i++)
	{
		for (int j = 0; j < *columna; j++)
		{
			Console::SetCursorPosition(i, j);
			/*
			1 = pared safe
			2 = piso safe
			3 = pared danger
			4 = piso danger
			5 = jugador
			6 = llave
			7 = tesoro
			8 = salida
			*/
			switch (matriz[i][j])
			{
			case 1:
				cout << (char)219;
				break;
			case 2:
				cout << (char)176;
				break;
			case 3:
				Console::ForegroundColor = ConsoleColor::DarkRed;
				cout << (char)219;
				break;
			case 4:
				Console::ForegroundColor = ConsoleColor::DarkRed;
				cout << (char)176;
				break;
			case 5:
				Console::ForegroundColor = ConsoleColor::Cyan;
				cout << (char)254;
				break;
			case 6:
				Console::ForegroundColor = ConsoleColor::Yellow;
				cout << 'L';
				break;
			case 7:
				Console::ForegroundColor = ConsoleColor::Magenta;
				cout << 'T';
				break;
			case 8:
				Console::ForegroundColor = ConsoleColor::Green;
				cout << 'S';
				break;
			}
			Console::ForegroundColor = ConsoleColor::Gray;
		}
	}
}

int** GenerarPieza(int *id_pieza)
{
	/*
	0 = nada
	1 = pared safe
	2 = piso safe
	3 = pared danger
	4 = piso danger
	5 = jugador
	6 = llave
	7 = tesoro
	8 = salida
	*/

	int **aux = new int*[5];
	int tipo = *id_pieza / 10; // decomposicion del primer digito

	for (int i = 0; i < 5; i++)
	{
		aux[i] = new int[5];
	}

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			switch (tipo)
			{
			case 1: // cruz
				if (i == 0 || i == 4)
				{
					if (j == 0 || j == 4)
						aux[i][j] = 1;
				}
				else
					aux[i][j] = 2;
				break;
			case 2: // L
				if (j == 0)
					aux[i][j] = 1;
				else if (i == 4)
					aux[i][j] = 1;
				else if (i == 0 && j == 4)
					aux[i][j] = 1;
				else
					aux[i][j] = 2;
				break;
			case 3: // recta
				if (i == 0 || i == 4)
					aux[i][j] = 1;
				else
					aux[i][j] = 2;
				break;
			case 4: // callejon
				if (i == 0 || i == 4)
					aux[i][j] = 1;
				else if (j == 4)
					aux[i][j] = 1;
				else
					aux[i][j] = 2;
				break;
			case 5: // T
				if (i == 4)
					aux[i][j] = 1;
				else if (i == 0 && (j == 0 || j == 4))
					aux[i][j] = 1;
				else
					aux[i][j] = 2;
				break;
			case 6: // vacio
				aux[i][j] = 0;
			}
		}
	}
	return aux;
}

void GenerarMapa(int **matriz, int *fila, int *columna)
{
	int counter = 0;
	for (int i = 0; i < (*fila / 3); i++)
	{
		for (int j = 0; j < (*columna / 3); j++)
		{
			matriz[i][j] = (counter % 5) + 1;
			counter++;
		}
		counter++;
	}
}

void RotarPieza(int **pieza, int *dir)
{
	int *dim; dim = new int; *dim = 3;
	int **aux; aux = CrearMatriz(dim, dim);
	int counter = *dir;

	while (counter > 0)
	{
		for (int i = 0; i < 3; i++)
			aux[i][2] = pieza[0][i];
		for (int i = 0; i < 3; i++)
			aux[i][1] = pieza[1][i];
		for (int i = 0; i < 3; i++)
			aux[i][0] = pieza[2][i];
		counter--;
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			pieza[i][j] = aux[i][j];
	}
}

int main()
{
	bool *es_enter, *es_esc, *tiene_llave, *uso_pocion;
	int *scancode, *menu_state, *game_state, *hp_jugador, *cant_pociones, *dmg, *id_enemigo, *hp_enemigo, *fila, *columna, **matriz, *fila_mapa, *columna_mapa;
	scancode = new int; // es el scancode de las teclas ingresadas
	menu_state = new int; // que opcion se selecciono en el menu
	es_enter = new bool; // si se presiono enter
	es_esc = new bool; // si se presiono esc
	game_state = new int; // que pantalla (menu, juego o ayuda) se esta mostrando
	tiene_llave = new bool; // si tiene la llave
	uso_pocion = new bool; // si uso una pocion
	cant_pociones = new int; // cantidad de pociones que tiene
	hp_jugador = new int; // la vida del jugador (0-5)
	dmg = new int; // el ataque del jugador
	id_enemigo = new int; // la identificacion del enemigo
	hp_enemigo = new int; // la vida del enemigo
	fila = new int; *fila = 50; // para fichas de 5x5
	columna = new int; *columna = 20; // para fichas de 5x5
	fila_mapa = new int; *fila_mapa = *fila / 5;
	columna_mapa = new int; *columna_mapa = *columna / 5;

	Console::CursorVisible = false;

	*es_enter = false;
	*es_esc = false;
	*menu_state = 0;
	*game_state = 0;
	*hp_jugador = 1; // en la version final esto va a ser 5...
	*cant_pociones = 3; // ...y esto 0 
	*dmg = 0;
	*tiene_llave = false;
	*uso_pocion = false;
	*id_enemigo = 0;

	MenuPrincipal(menu_state, game_state);

	matriz = CrearMatriz(fila, columna); // el mapa que sa va a imprimir
	GenerarMatriz(matriz, fila, columna);

	while (true)
	{
		Captura(menu_state, es_enter, scancode, es_esc, game_state, uso_pocion, id_enemigo);

		switch (*game_state)
		{
		case 0:
			MenuPrincipal(menu_state, game_state);
			break;
		case 1:
			Dibujante(game_state, hp_jugador, cant_pociones, dmg, tiene_llave, uso_pocion, id_enemigo, hp_enemigo, matriz, fila, columna);
			break;
		case 2:
			PantallaAyuda(game_state);
			break;
		case 3:
			exit(0); // cierra el programa con codigo 0 (return 0)
			break;
		}
	}

	delete es_enter, es_esc, scancode, menu_state, game_state, tiene_llave, uso_pocion, cant_pociones, hp_jugador, dmg, id_enemigo, hp_enemigo;
	return 0;
}