#ifndef PROJETO_H
#define PROJETO_H

#include <stdio.h>
#include <string.h>

#define Total_Cliente 10
#define TOTAL_ADM 10
#define TOTAL_CRIPTOMOEDAS 10
#define TOTAL_CRIPTO_SUPORTADAS 10

typedef struct {
    char nome_completo[100];
    char cpf[12];
    char dataNascimento[11];
    char endereco[150];
    char telefone[20];
    float saldo;
    float bitcoin;
    float ethereum;
    float ripple;
    char senha[20];
} Cliente;

typedef struct {
    Cliente clientes[Total_Cliente];
    int qtd_cliente;
} ListaDeCliente;

typedef struct {
    char nome_completo[100];
    char cpf[12];
    char dataNascimento[11];
    char endereco[150];
    char telefone[20];
    char senha[20];
} Adm;

typedef struct {
    char nome[50];     // Nome da criptomoeda
    char simbolo[10];  // Símbolo (ex: BTC)
    double preco;      // Preço da criptomoeda
} Criptomoeda;

typedef struct {
    Criptomoeda criptomoedas[TOTAL_CRIPTOMOEDAS];
    int qtd_criptomoedas;
} ListaDeCriptomoedas;

typedef struct {
    Adm adm[TOTAL_ADM];
    int qtd_adm;
} ListaDeAdm;

void exibirMenu(); //feito
void exibirMenuAdmin(); //feito

int criarCliente(ListaDeCliente *lt); //feito
int deletarCliente(ListaDeCliente *lt); //feito

int listarCliente(ListaDeCliente *lt); //feito

int consultarSaldo(ListaDeCliente *lt); //feito
int depositar(ListaDeCliente *lt); //feito
int sacar(ListaDeCliente *lt); //feito
int comprarCripto(ListaDeCliente *lt, ListaDeCriptomoedas *lc); //feito
int venderCripto(ListaDeCliente *lt, ListaDeCriptomoedas *lc); //feito
double gerarVariacao(); //feito
int atualizarCotacao(double *bitcoin_price, double *ethereum_price, double *ripple_price); //feito
int loginCliente(ListaDeCliente *lt); //feito

int carregarClientes(ListaDeCliente *lt, const char *nome); //feito
int salvarClientes(ListaDeCliente *lt, const char *nome); //feito

int loginAdmin(ListaDeAdm *la);  //feito
int criarAdmin(ListaDeAdm *la); //feito
int carregarAdmins(ListaDeAdm *la, const char *nome); //feito
int salvarAdmins(ListaDeAdm *la, const char *nome); //feito

int criarCriptomoeda(ListaDeCriptomoedas *lc); //feito
int excluirCriptomoeda(ListaDeCriptomoedas *lc); //feito
void listarCriptomoedas(ListaDeCriptomoedas *lc);  //feito
int carregarCriptomoedas(ListaDeCriptomoedas *lc, const char *nome); //feito
int salvarCriptomoedas(ListaDeCriptomoedas *lc, const char *nome); //feito

int consultarExtrato(ListaDeCliente *lt);
void adicionarCriptosPadrao(ListaDeCriptomoedas *lc);
int consultarExtratoADM(ListaDeCliente *lt);
int consultarSaldoAdmin(ListaDeCliente *lt);

#endif