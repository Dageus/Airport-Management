/*
*João Miguel Nogueira
*1103808
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 17317			/* tamanho da hash table */
#define MAX_MEMORIA 65535	/* memória máxima que o programa pode utilizar */

#define MIN_CODIGO_RESERVA 10 	/* dimensão mínima do código de reserva */

#define MAX_NUM_AEROPORTOS 40	/* número máximo de areoportos */
#define MAX_NUM_VOOS 30000	/* número máximo de voos */

#define MAX_CODIGO_AEROPORTO 6	/* dimensão do código do aeroporto */
#define MAX_NOME_PAIS 31	/* dimensão do nome do pais */
#define MAX_NOME_CIDADE 51	/* dimensão do nome da cidade */

#define MAX_CODIGO_VOO 7	/* dimensão do código do voo */
#define MAX_DATA 11			/* dimensão da data */
#define MAX_HORA 6			/* dimensão da hora */

#define NAO_EXISTE -1		/* código de erro */

#define ANO_INICIO 2022		/* ano inicial do sistema */
#define DIAS_ANO 365		/* número de dias no ano */
#define HORAS_DIA 24		/* número de horas por dia */
#define MINUTOS_HORA 60		/* número de minutos numa hora */
#define MINUTOS_DIA 1440	/* número de minutos do dia */

#define TRUE 1			/* verdadeiro */
#define FALSE 0			/* falso */

/* Tipos de Dados */

typedef struct {
	char id[MAX_CODIGO_AEROPORTO];
	char pais[MAX_NOME_PAIS];
	char cidade[MAX_NOME_CIDADE];
	int numVoos;
} Aeroporto;

typedef struct {
	int dia;
	int mes;
	int ano;
} Data;

typedef struct {
	int hora;
	int minuto;
} Hora;

typedef struct {
	char id[MAX_CODIGO_VOO];
	char partida[MAX_CODIGO_AEROPORTO];
	char chegada[MAX_CODIGO_AEROPORTO];
	Data data;
	Hora hora;
	Hora duracao;
	int capacidade;
	int horaPartida;
	int horaChegada;
	int numReservasTotal;  
} Voo;

typedef struct reserva{
	char codigo_voo[MAX_CODIGO_VOO];
	Data data;
	char *codigo_reserva;
	int capacidade;
} *Reserva;

typedef struct lista_reservas {
	Reserva reserva;
	struct lista_reservas* proximo;
} *listaReservas;

/* Variaveis Globais */
int _numReservas = 0; 	/* numero de reservas introduzidas */
listaReservas *_tabelaCodigoVoo; 	/* hash table de reservas, code = codigo de voo */
listaReservas *_tabelaCodigoReservas; 	/* hash table de reservas, code = codigo de reserva */


int _numAeroportos = 0;		/* número de aeroportos introduzidos */
Aeroporto _aeroportos[MAX_NUM_AEROPORTOS];	/* vetor de aeroportos */

int _numVoos = 0		/* número de voos introduzidos */;
Voo _voos[MAX_NUM_VOOS];	/* vetor de voos */

Data _hoje = { 1, 1, 2022 };	/* data atual do sistema */

const int _diasMesAc[] =	/* dias acumulados por mês (jan=1) */
	{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };


/* Funções referentes às duas Hash Tables */

void init(){
	int i;

	_tabelaCodigoReservas = (listaReservas*) malloc(SIZE*sizeof(listaReservas));
	_tabelaCodigoVoo = (listaReservas*) malloc(SIZE*sizeof(listaReservas));
	for (i = 0; i < SIZE; i++){
		_tabelaCodigoReservas[i] = NULL;
		_tabelaCodigoVoo[i] = NULL;
	}
	return;
}


int obterHash(char* codigo){
	int c, a = 31414, b = 27183;
	
	for (c = 0; *codigo != '\0'; ++codigo, a = a * b % (SIZE - 1)){
		c = (a * c + *codigo) % SIZE;
	}
	if (c < 0)
		return -c;
	else
		return c;
}


listaReservas NEW(Reserva r){
	listaReservas t = (listaReservas) malloc(sizeof(struct lista_reservas));
	t->reserva = r;
	t->proximo = NULL;
	return t;
}

listaReservas adicionaNovaReservaInicio(listaReservas head, Reserva r){
	listaReservas t = NEW(r);
	t->proximo = head;
	return t;
}


/* Funções Leitura */

Hora leHora(){
	Hora h;
	scanf("%d:%d", &h.hora, &h.minuto);
	return h;
}


Data leData(){
	Data d;
	scanf("%d-%d-%d", &d.dia, &d.mes, &d.ano);
	return d;
}


int leProximaPalavra(char str[]) {
	char c = getchar();
	int i = 0;
	while (c == ' ' || c == '\t')
		c = getchar();
	while (c != ' ' && c != '\t' && c != '\n') {
		str[i++] = c;
		c = getchar();
	}
	str[i] = '\0';
	return (c == '\n');
}


void lePalavraAteFimDeLinha(char str[]) {
	char c = getchar();
	int i = 0;
	while (c == ' ' || c == '\t')
		c = getchar();
	while (c != '\n') {
		str[i++] = c;
		c = getchar();
	}
	str[i] = '\0';
}


void leAteFimDeLinha() {
	char c = getchar();
	while (c != '\n')
		c = getchar();
}


/* Funções Adicionais */


int verificaDigito(char a){
    if (a >= '0' && a <= '9'){
        return TRUE;
    }
    return FALSE;
}


int verificaMaiuscula(char a){
    if (a >= 'A' && a <= 'Z'){
        return TRUE;
    }
    return FALSE;
}


/* Funções Memória */


int excedeMemoria(char* ptr){
	if (ptr == NULL){
		return TRUE;
	} 
	return FALSE;
}


void limpa_memoria(){
	long int i;
	listaReservas t;
	
	for (i = 0; i < SIZE; i++){
		if (_tabelaCodigoReservas[i] != NULL)
			for (t = _tabelaCodigoReservas[i]; t != NULL; t = t->proximo){
				free(t->reserva->codigo_reserva);
				free(t->reserva);
			}
	}
}


/* Funcoes Datas e Horas */


void mostraData(Data d) {
	if (d.dia < 10)
		printf("0");
	printf("%d-", d.dia);
	if (d.mes < 10)
		printf("0");
	printf("%d-%d", d.mes, d.ano);
}


void mostraHora(Hora h) {
	if (h.hora < 10)
		printf("0");
	printf("%d:", h.hora);
	if (h.minuto < 10)
		printf("0");
	printf("%d", h.minuto);
}


int converteDataNum(Data d) {
	return (d.ano - ANO_INICIO) * DIAS_ANO + _diasMesAc[d.mes - 1] +
		d.dia - 1;
}


int converteHoraNum(Hora h) {
	return ((h.hora * MINUTOS_HORA) + h.minuto);
}


int converteDataHoraNum(Data d, Hora h) {
	return converteDataNum(d) * MINUTOS_DIA + converteHoraNum(h);
}


Hora converteNumHora(int num) {
	Hora h;
	h.minuto = num % MINUTOS_HORA;
	h.hora = ((num - h.minuto) / MINUTOS_HORA) % HORAS_DIA;
	return h;
}


Data converteNumData(int num) {
	Data d;
	int i = 0;
	num = (num - (num % MINUTOS_DIA)) / MINUTOS_DIA;
	d.ano = (num / DIAS_ANO) + ANO_INICIO;
	num = num - ((d.ano - ANO_INICIO) * DIAS_ANO);
	while (i <= 11 && num >= _diasMesAc[i])
		i++;
	d.mes = i;
	d.dia = num - _diasMesAc[i - 1] + 1;
	return d;
}


int validaData(Data d) {
	int numData = converteDataNum(d);
	Data proximoAno = _hoje;
	proximoAno.ano++;
	return !(numData < converteDataNum(_hoje)
		|| numData > converteDataNum(proximoAno));
}


int validaHora(Hora h) {
	return !(h.hora > 12 || (h.hora == 12 && h.minuto > 0));
}


/* Algoritmo de ordenação BubbleSort */


void bubbleSort(int indexes[], int size, int (*cmpFunc) (int a, int b)) {
  int i, j, done;
  
  for (i = 0; i < size-1; i++){
    done=1;
    for (j = size-1; j > i; j--) 
      if ((*cmpFunc)(indexes[j-1], indexes[j])) {
	int aux = indexes[j];
	indexes[j] = indexes[j-1];
	indexes[j-1] = aux;
	done=0;
      }
    if (done) break;
  }
}


/* Funcoes Aeroportos */


int aeroportoInvalido(char id[]) {
	int i;
	
	if (strlen(id) < 3)
		return TRUE;
	for (i = 0; id[i] != '\0'; i++)
		if (!(id[i] >= 'A' && id[i] <= 'Z') || !(id[i] >= 'a' && id[i] <= 'z'))
			return TRUE;
	return FALSE;
}


int encontraAeroporto(char id[]) {
	int i;
	for (i = 0; i < _numAeroportos; i++)
		if (!strcmp(id, _aeroportos[i].id))
			return i;
	return NAO_EXISTE;
}


void adicionaAeroporto() {
	Aeroporto a;
	char codigo[6]

	leProximaPalavra(codigo);
	leProximaPalavra(a.pais);
	lePalavraAteFimDeLinha(a.cidade);

	a.id = (char*) malloc(sizeof(char) * (strelen(codigo) + 1));

	if (aeroportoInvalido(a.id))
		printf("invalid airport ID\n");
	else if (_numAeroportos == MAX_NUM_AEROPORTOS)
		printf("too many airports\n");
	else if (encontraAeroporto(a.id) != NAO_EXISTE)
		printf("duplicate airport\n");
	else {
		strcpy(_aeroportos[_numAeroportos].id, a.id);
		strcpy(_aeroportos[_numAeroportos].pais, a.pais);
		strcpy(_aeroportos[_numAeroportos].cidade, a.cidade);
		_aeroportos[_numAeroportos].numVoos = 0;
		_numAeroportos++;
		printf("airport %s\n", a.id);
	}
}


void mostraAeroporto(int index) {
	printf("%s %s %s %d\n", _aeroportos[index].id,
	       _aeroportos[index].cidade, _aeroportos[index].pais,
	       _aeroportos[index].numVoos);
}


int cmpAeroportos(int a, int b) {
  return (strcmp(_aeroportos[a].id, _aeroportos[b].id) > 0);
}


void listaTodosAeroportos() {
	int i;
	int indexAeroportos[MAX_NUM_AEROPORTOS];

	for (i = 0; i < _numAeroportos; i++)
		indexAeroportos[i] = i;

	bubbleSort(indexAeroportos, _numAeroportos, cmpAeroportos);

	for (i = 0; i < _numAeroportos; i++)
		mostraAeroporto(indexAeroportos[i]);
}


void listaAeroportos() {
	char id[MAX_CODIGO_AEROPORTO];
	int indexAeroporto, ultima = 0;

	ultima = leProximaPalavra(id);
	if (strlen(id) == 0)
		listaTodosAeroportos();
	else {
		while (strlen(id) != 0) {
			indexAeroporto = encontraAeroporto(id);
			if (indexAeroporto == NAO_EXISTE)
				printf("%s: no such airport ID\n", id);
			else
				mostraAeroporto(indexAeroporto);
			if (!ultima)
				ultima = leProximaPalavra(id);
			else
				break;
		}
	}
}



/* Funcoes Voos */

void mostraVoo(int index) {
	printf("%s %s %s ", _voos[index].id, _voos[index].partida,
	       _voos[index].chegada);
	mostraData(_voos[index].data);
	printf(" ");
	mostraHora(_voos[index].hora);
	printf("\n");
}

void mostraVooPartida(int index) {
	printf("%s %s ", _voos[index].id, _voos[index].chegada);
	mostraData(_voos[index].data);
	printf(" ");
	mostraHora(_voos[index].hora);
	printf("\n");
}

void mostraVooChegada(int index) {
	Hora h = converteNumHora(_voos[index].horaChegada);
	printf("%s %s ", _voos[index].id, _voos[index].partida);
	mostraData(converteNumData(_voos[index].horaChegada));
	printf(" ");
	mostraHora(h);
	printf("\n");
}


int encontraVoo(char id[], Data d) {
	int numData = converteDataNum(d);
	int i;

	for (i = 0; i < _numVoos; i++)
		if (!strcmp(id, _voos[i].id)
		    && numData == converteDataNum(_voos[i].data))
			return i;
	return NAO_EXISTE;
}


int validaIDVoo(char id[]) {
	int i = 0, l = strlen(id);
	if (l < 3)
		return FALSE;
	for (i = 0; i < 2; i++)
		if (!(id[i] >= 'A' && id[i] <= 'Z'))
			return FALSE;

	while (id[i] != '\0') {
		if (!(id[i] >= '0' && id[i] <= '9'))
			return FALSE;
		i++;
	}
	return TRUE;
}

int validaVoo(Voo v) {
	if (validaIDVoo(v.id) == FALSE)
		printf("invalid flight code\n");
	else if (encontraVoo(v.id, v.data) != NAO_EXISTE)
		printf("flight already exists\n");
	else if (encontraAeroporto(v.partida) == NAO_EXISTE)
		printf("%s: no such airport ID\n", v.partida);
	else if (encontraAeroporto(v.chegada) == NAO_EXISTE)
		printf("%s: no such airport ID\n", v.chegada);
	else if (_numVoos == MAX_NUM_VOOS)
		printf("too many flights\n");
	else if (validaData(v.data) == FALSE)
		printf("invalid date\n");
	else if (validaHora(v.duracao) == FALSE)
		printf("invalid duration\n");
	else if (v.capacidade < 10)
		printf("invalid capacity\n");
	else
		return TRUE;
	return FALSE;
}

void criaVoo(Voo v) {
	int i;
	strcpy(_voos[_numVoos].id, v.id);
	strcpy(_voos[_numVoos].partida, v.partida);
	strcpy(_voos[_numVoos].chegada, v.chegada);
	_voos[_numVoos].data = v.data;
	_voos[_numVoos].hora = v.hora;
	_voos[_numVoos].duracao = v.duracao;
	_voos[_numVoos].capacidade = v.capacidade;
	_voos[_numVoos].numReservasTotal = 0;
	_voos[_numVoos].horaPartida =
		converteDataHoraNum(_voos[_numVoos].data,
				    _voos[_numVoos].hora);
	_voos[_numVoos].horaChegada =
		_voos[_numVoos].horaPartida +
		converteHoraNum(_voos[_numVoos].duracao);
	for (i = 0; i < _numAeroportos; i++)
		if (!strcmp(_voos[_numVoos].partida, _aeroportos[i].id))
			_aeroportos[i].numVoos++;
	_numVoos++;
}

void adicionaListaVoos() {
	Voo v;
	int i;

	if (leProximaPalavra(v.id)) {
		for (i = 0; i < _numVoos; i++)
			mostraVoo(i);
		return;
	} else {
		leProximaPalavra(v.partida);
		leProximaPalavra(v.chegada);
		v.data = leData();
		v.hora = leHora();
		v.duracao = leHora();
		scanf("%d", &v.capacidade);
		leAteFimDeLinha();
	}
	if (validaVoo(v))
		criaVoo(v);
}

int cmpVoosPartida(int a, int b) {
	return (_voos[a].horaPartida > _voos[b].horaPartida);
}


int cmpVoosChegada(int a, int b) {
	return (_voos[a].horaChegada > _voos[b].horaChegada);
}


void listaVoosPartida() {
	int indexVoos[MAX_NUM_VOOS], i, n = 0;
	char partida[MAX_CODIGO_AEROPORTO];
	int dataNum = converteDataNum(_hoje);

	lePalavraAteFimDeLinha(partida);

	if (encontraAeroporto(partida) == NAO_EXISTE)
		printf("%s: no such airport ID\n", partida);

	for (i = 0; i < _numVoos; i++) {
		if (!strcmp(_voos[i].partida, partida) && dataNum == converteDataNum(_voos[i].data))
			indexVoos[n++] = i;
	}

	bubbleSort(indexVoos, n, cmpVoosPartida);

	for (i = 0; i < n; i++)
		mostraVooPartida(indexVoos[i]);
}


void listaVoosChegada() {
	int indexVoos[MAX_NUM_VOOS], i, n = 0;
	char chegada[MAX_CODIGO_AEROPORTO];

	lePalavraAteFimDeLinha(chegada);

	if (encontraAeroporto(chegada) == NAO_EXISTE)
		printf("%s: no such airport ID\n", chegada);

	for (i = 0; i < _numVoos; i++) {
		if (!strcmp(_voos[i].chegada, chegada))
			indexVoos[n++] = i;
	}

	bubbleSort(indexVoos, n, cmpVoosChegada);

	for (i = 0; i < n; i++)
		mostraVooChegada(indexVoos[i]);
}


void alteraData() {
	Data d;

	d = leData();
	leAteFimDeLinha();
	if (validaData(d) == FALSE)
		printf("invalid date\n");
	else {
		_hoje = d;
		mostraData(_hoje);
		printf("\n");
	}
}


/* Algoritmo de ordenação Merge Sort */

int cmpReservas(char* codigo1, char* codigo2){
	return (strcmp(codigo1, codigo2) <= 0);
}


void merge(Reserva *r, int left, int m, int right){
	int i, j, k;
    int n1 = m - left + 1;
    int n2 = right - m;
  
    Reserva *L, *R;

	L = (Reserva*) malloc(sizeof(Reserva)*n1);
	R = (Reserva*) malloc(sizeof(Reserva) * n2);
  
    for (i = 0; i < n1; i++)
        L[i] = r[left + i];
    for (j = 0; j < n2; j++)
        R[j] = r[m + 1 + j];
  
    i = 0; 
    j = 0; 
    k = left; 
    while (i < n1 && j < n2) {
        if (cmpReservas(L[i]->codigo_reserva, R[j]->codigo_reserva)) {
            r[k] = L[i];
            i++;
        }
        else {
            r[k] = R[j];
            j++;
        }
        k++;
    }
  
    while (i < n1) {
        r[k] = L[i];
        i++;
        k++;
    }
  
    while (j < n2) {
        r[k] = R[j];
        j++;
        k++;
    }
}


void mergeSort(Reserva *r, int left, int right){
	int m = left + (right - left)/2;
	if (left < right){
		mergeSort(r, left, m);
		mergeSort(r, m + 1, right);
		merge(r, left, m, right);
	}
}


/* Funções Reserva */


int validaCodigo(char* codigo){
	int i = 0;
    if (strlen(codigo) < MIN_CODIGO_RESERVA){
        return TRUE;
    }
	while (codigo[i] != '\0'){
		if (!verificaDigito(codigo[i]) && !verificaMaiuscula(codigo[i]))
			return TRUE;
		++i;
	}
	return FALSE;
}


int duplicadaReserva(char* codigo_reserva, char* codigo_voo){
	listaReservas t;
	int i = obterHash(codigo_reserva);
	int j = obterHash(codigo_voo);

	for (t = _tabelaCodigoReservas[i]; t != NULL; t = t->proximo){
		if (!strcmp(t->reserva->codigo_reserva, codigo_reserva)){
			return TRUE;
        }
    }
	
	for (t = _tabelaCodigoVoo[j]; t != NULL; t = t->proximo){
		if (!strcmp(t->reserva->codigo_reserva, codigo_reserva)){
			puts("retorna TRUE");
			return TRUE;
        }
	}
	return FALSE;
}


int encontraComparaVoo(Data data, int capacidade, char* codigo_voo){
	int numData = converteDataNum(data);
	int i;
	for (i = 0; i < _numVoos; i++)
		if (!strcmp(codigo_voo, _voos[i].id)
		&& numData == converteDataNum(_voos[i].data))
			return (_voos[i].numReservasTotal + capacidade > _voos[i].capacidade);
    return FALSE;
}


int validaReserva(Reserva r){
	if (validaCodigo(r->codigo_reserva))
		printf("invalid reservation code\n");
	else if (encontraVoo(r->codigo_voo, r->data) == NAO_EXISTE)
		printf("%s: flight does not exist\n", r->codigo_voo);
	else if (duplicadaReserva(r->codigo_reserva, r->codigo_voo))
		printf("%s: flight reservation already used\n", r->codigo_reserva);
	else if (encontraComparaVoo(r->data, r->capacidade, r->codigo_voo))
		printf("too many reservations\n");
	else if (validaData(r->data) == FALSE)
		printf("invalid date\n");
	else if (r->capacidade <= 0)
		printf("invalid passenger number\n");
    else
        return TRUE;
    return FALSE;
}


void criaReserva(Reserva r){
	int m;
	int numData = converteDataNum(r->data);
	int i = obterHash(r->codigo_voo);
	int j = obterHash(r->codigo_reserva);
	
	_tabelaCodigoReservas[j] = adicionaNovaReservaInicio(_tabelaCodigoReservas[j], r);
	_tabelaCodigoVoo[i] = adicionaNovaReservaInicio(_tabelaCodigoVoo[i], r);
	for (m = 0; m < _numVoos; ++m)
		if (!strcmp(r->codigo_voo, _voos[m].id)
		&& numData == converteDataNum(_voos[m].data))
			_voos[m].numReservasTotal += r->capacidade;
	_numReservas ++;
}


void mostraReserva(Reserva r){
	printf("%s %d\n", r->codigo_reserva, r->capacidade);
}


void listaTodasReservas(char* codigo, Data data){
	listaReservas t;
	Reserva *arr;
	int numData = converteDataNum(data);
	int i = 0, m;
	int j = obterHash(codigo);

	if (_tabelaCodigoVoo[j] == NULL)
		return;
	else {
		arr = (Reserva*) malloc(sizeof(Reserva) * _numReservas);

		for (t = _tabelaCodigoVoo[j]; t != NULL; t = t->proximo){
			if (numData == converteDataNum(t->reserva->data)){
				arr[i++] = t->reserva;
			}
		}
		mergeSort(arr, 0, i - 1);
		for (m = 0; m < i; ++m){
			mostraReserva(arr[m]);
		}
	}
}


int validaLista(char* codigo, Data data){
	if (encontraVoo(codigo, data) == NAO_EXISTE)
		printf("%s: flight does not exist\n", codigo);
	else if (validaData(data) == FALSE)
		printf("invalid date\n");
	else
		return TRUE;
	return FALSE; 
}


void reserva(){
	Reserva r = (Reserva) malloc(sizeof(struct reserva));
	char codigo[65536]; 
    
	leProximaPalavra(r->codigo_voo);
	r->data = leData();
	if (leProximaPalavra(codigo)){
		if (validaLista(r->codigo_voo, r->data))
			listaTodasReservas(r->codigo_voo, r->data);
	}
	else {
        /*guardar dados*/
        r->codigo_reserva = (char*) malloc(sizeof(char)*(strlen(codigo) + 1));
		if (excedeMemoria(r->codigo_reserva)){
			printf("No memory\n");
			limpa_memoria();
			exit(TRUE);
		}
        strcpy(r->codigo_reserva, codigo);
		scanf("%d", &r->capacidade);
		leAteFimDeLinha();
		if (validaReserva(r))
			criaReserva(r);
		r = NULL;
    }
}


/* Funcoes Eliminar */


void apagaVoo(int index){
	int i;

	for (i = 0; i < _numAeroportos; i++)
		if (!strcmp(_voos[index].partida, _aeroportos[i].id))
			_aeroportos[i].numVoos--;
	for (i = index; i < _numVoos - 1; i++)
		_voos[i] = _voos[i + 1];
}


void eliminaVoo(char* codigo){
	int i, j = TRUE;
	int dataNum = converteDataNum(_hoje);

	for (i = 0; i < _numVoos; ++i){
		if (!strcmp(codigo, _voos[i].id) && (dataNum > converteDataNum(_voos[i].data))){
			j = FALSE;
			apagaVoo(i);
			--i;
			--_numVoos;
		}
		
		else if (j && i == _numVoos - 1)
			printf("not found\n");
	}
}


void reduzReservas(char* codigo, int capacidade){
	int i;

	for(i = 0; i < _numVoos; i++){
		if (!strcmp(_voos[i].id, codigo)){
			_voos[i].numReservasTotal -= capacidade;
		}
	}
}


void eliminaReservaCodigoVoo(char* codigo_voo, char* codigo_reserva){
	listaReservas t, prev;
	int i = obterHash(codigo_voo);

	for (t = _tabelaCodigoVoo[i], prev = NULL; t != NULL ||
	prev->proximo != NULL; prev = t, t = t->proximo){
		if (!strcmp(t->reserva->codigo_reserva, codigo_reserva)){
			if (t == _tabelaCodigoVoo[i]){
				_tabelaCodigoVoo[i] = t->proximo;
			}
			else{
				prev->proximo = t->proximo;
			}
		}
	}
}


void eliminaReserva(char* codigo){
	listaReservas t, prev;
	int i = obterHash(codigo);
	int j = 0;

	if (_tabelaCodigoReservas[i] == NULL){
		printf("not found\n");
		return;
	}

	for (t = _tabelaCodigoReservas[i], prev = NULL; t != NULL ||
	prev->proximo != NULL; prev = t, t = t->proximo){
		if(!strcmp(t->reserva->codigo_reserva, codigo)){
			/*apagar reserva*/
			reduzReservas(t->reserva->codigo_voo, t->reserva->capacidade);
			eliminaReservaCodigoVoo(t->reserva->codigo_voo, codigo);
			if (t == _tabelaCodigoReservas[i]){
				_tabelaCodigoReservas[i] = t->proximo;
			}
			else{
				prev->proximo = t->proximo;
			}
			free(t->reserva->codigo_reserva);
			free(t->reserva);
			_numReservas--;
			break;
		}
		else if (j == _numReservas)
			printf("not found\n");
		j++;
	}
}


void eliminaReservaCodigoReserva(char* codigo){
	listaReservas t, prev;
	int i = obterHash(codigo);

	for (t = _tabelaCodigoReservas[i], prev = NULL; t != NULL ||
	prev->proximo != NULL; prev = t, t = t->proximo){
		if (!strcmp(t->reserva->codigo_reserva, codigo)){
			if (t == _tabelaCodigoReservas[i]){
				_tabelaCodigoReservas[i] = t->proximo;
			}
			else{
				prev->proximo = t->proximo;
			}
		}
	}
}


void eliminaReservas(char* codigo, int index){ 
	listaReservas t, prev;
	int i = 0;

	for (t = _tabelaCodigoVoo[index], prev = NULL; t != NULL ||
	prev->proximo != NULL; prev = t, t = t->proximo){
		if (!strcmp(t->reserva->codigo_voo, codigo)){
			if (t == _tabelaCodigoVoo[index]){
				_tabelaCodigoVoo[index] = t->proximo;
			}
			else{
				prev->proximo = t->proximo;
			}
			eliminaReservaCodigoReserva(t->reserva->codigo_reserva);
			free(t->reserva->codigo_reserva);
			free(t->reserva);
		}
		else if (i == _numReservas - 1)
			printf("not found\n");
		i++;
	}
}


void eliminaCodigo(char* codigo){
	int i;

	if(strlen(codigo) < MIN_CODIGO_RESERVA) { /*apaga-se o voo e as _reservas associadas*/
		i = obterHash(codigo);
		eliminaVoo(codigo);
		if (_tabelaCodigoVoo[i] != NULL)
			eliminaReservas(codigo, i);
	}
	else{ 						 /*apaga-se a reserva*/
		eliminaReserva(codigo);
	}
}


void eliminar(){
	char codigo[65536];

	lePalavraAteFimDeLinha(codigo);

	eliminaCodigo(codigo);
}


int main(){
    int c;

	init();

	while ((c = getchar()) != EOF) {
		switch (c) {
            case 'q': limpa_memoria(); 
				exit(FALSE);
				break;
            case 'a': adicionaAeroporto();
				break;
			case 'l': listaAeroportos();
				break;
            case 'v': adicionaListaVoos();
				break;
			case 'p': listaVoosPartida();
				break;
			case 'c': listaVoosChegada();
				break;
			case 't': alteraData();
				break;
        /*novos comandos*/
            case 'r': reserva();
                break;
            case 'e': eliminar();
                break;
            default: printf("Invalid comand: %c\n", c);
        }
	}

	return 0;
}