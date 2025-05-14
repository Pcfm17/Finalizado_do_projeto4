#include "projeto.h"
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOME_ARQUIVO_CLIENTES "clientes.txt"
#define NOME_ARQUIVO_ADMINS "admins.txt"
#define NOME_ARQUIVO_CRIPTOMOEDAS "criptomoedas.txt"
#define MAX_LINE_SIZE 256

void limparBuffer() {
    while (getchar() != '\n');
}

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");

    ListaDeCliente lista_clientes;
    lista_clientes.qtd_cliente = 0;
    carregarClientes(&lista_clientes, NOME_ARQUIVO_CLIENTES);

    ListaDeAdm lista_admins;
    lista_admins.qtd_adm = 0;
    carregarAdmins(&lista_admins, NOME_ARQUIVO_ADMINS);

    ListaDeCriptomoedas lista_criptomoedas;
    lista_criptomoedas.qtd_criptomoedas = 0;
    carregarCriptomoedas(&lista_criptomoedas, NOME_ARQUIVO_CRIPTOMOEDAS);
    printf("Criptomoedas carregadas inicialmente: %d\n", lista_criptomoedas.qtd_criptomoedas);
    adicionarCriptosPadrao(&lista_criptomoedas);

    double bitcoin_price = 30000.0;
    double ethereum_price = 2000.0;
    double ripple_price = 0.5;

    atualizarCotacao(&bitcoin_price, &ethereum_price, &ripple_price);

    printf("========================================================\n");
    printf(" Bem-vindo ao Sistema de Gerenciamento de Criptomoedas\n");
    printf("========================================================\n\n");

    int tipo_usuario;
    printf("Você é (1 - Cliente ou 2 - Administrador)? Digite o número: ");
    if (scanf("%d", &tipo_usuario) != 1 || (tipo_usuario != 1 && tipo_usuario != 2)) {
        printf("Entrada inválida.\n");
        return 1;
    }
    limparBuffer();  // Limpar o buffer após ler a escolha do usuário

    if (tipo_usuario == 1) {
        printf("\n--- Módulo do Cliente ---\n");
        printf("Por favor, faça o login para continuar.\n\n");

        int pergunta_cliente_login;
        printf("Você possui um login? [1. Sim e 2. Não] Digite o número: ");
        if (scanf("%d", &pergunta_cliente_login) != 1 || (pergunta_cliente_login != 1 && pergunta_cliente_login != 2)) {
            printf("Entrada inválida.\n");
            return 1;
        }
        limparBuffer();  // Limpar o buffer após ler a escolha

        switch (pergunta_cliente_login) {
            case 1:
                printf("LOGIN:\n");
                if (loginCliente(&lista_clientes) != 0) {
                    printf("Falha no login ou programa encerrado.\n");
                    return 1;
                }
                break;
            case 2:
                printf("CADASTRO:\n");
                if (criarCliente(&lista_clientes) != 0) {
                    printf("Falha no cadastro.\n");
                    return 1;
                }
                printf("LOGIN:\n");
                if (loginCliente(&lista_clientes) != 0) {
                    printf("Falha no login ou programa encerrado.\n");
                    return 1;
                }
                break;
            default:
                printf("Opção inválida.\n");
                return 1;
        }

        int escolha_cliente;
        do {
            exibirMenu();
            printf("Escolha uma Opção: ");
            if (scanf("%d", &escolha_cliente) != 1) {
                printf("Entrada inválida.\n");
                limparBuffer();  // Limpar buffer caso a entrada seja inválida
                continue;
            }
            limparBuffer();  // Limpar o buffer após ler a escolha

            switch (escolha_cliente) {
                case 1:
                    criarCliente(&lista_clientes);
                    break;
                case 2:
                    deletarCliente(&lista_clientes);
                    break;
                case 3:
                    carregarClientes(&lista_clientes, NOME_ARQUIVO_CLIENTES);
                    listarCliente(&lista_clientes);
                    break;
                case 4:
                    consultarSaldo(&lista_clientes);
                    break;
                case 5:
                    depositar(&lista_clientes);
                    break;
                case 6:
                    sacar(&lista_clientes);
                    break;
                case 7:
                    comprarCripto(&lista_clientes, &lista_criptomoedas);
                    break;
                case 8:
                    venderCripto(&lista_clientes, &lista_criptomoedas);
                    break;
                case 9:
                    atualizarCotacao(&bitcoin_price, &ethereum_price, &ripple_price);
                    break;
                case 10:
                    consultarExtrato(&lista_clientes);
            		break;
                case 11:
                    printf("Encerrando o programa e salvando os dados...\n");
                    salvarClientes(&lista_clientes, NOME_ARQUIVO_CLIENTES);
                    salvarAdmins(&lista_admins, NOME_ARQUIVO_ADMINS);
                    salvarCriptomoedas(&lista_criptomoedas, NOME_ARQUIVO_CRIPTOMOEDAS);
                    break;
                default:
                    printf("Opção inválida!\n");
            }
        } while (escolha_cliente != 11);
    } else if (tipo_usuario == 2) {
        printf("\n--- Módulo do Administrador ---\n");
        printf("Por favor, faça o login como administrador.\n\n");

        int pergunta_admin_login;
        printf("Você possui um login? [1. Sim e 2. Não] Digite o número: ");
        if (scanf("%d", &pergunta_admin_login) != 1 || (pergunta_admin_login != 1 && pergunta_admin_login != 2)) {
            printf("Entrada inválida.\n");
            return 1;
        }
        limparBuffer();  // Limpar o buffer após ler a escolha

        switch (pergunta_admin_login) {
            case 1:
                printf("LOGIN:\n");
                if (loginAdmin(&lista_admins) != 0) {
                    printf("Falha no login ou programa encerrado.\n");
                    return 1;
                }
                break;
            case 2:
                printf("CADASTRO:\n");
                if (criarAdmin(&lista_admins) != 0) {
                    printf("Falha no cadastro.\n");
                    return 1;
                }
                printf("LOGIN:\n");
                if (loginAdmin(&lista_admins) != 0) {
                    printf("Falha no login ou programa encerrado.\n");
                    return 1;
                }
                break;
            default:
                printf("Opção inválida.\n");
                return 1;
        }

        int escolha_admin;
        do {
            exibirMenuAdmin(); 
            printf("Escolha uma Opção: ");
            if (scanf("%d", &escolha_admin) != 1) {
                printf("Entrada inválida.\n");
                limparBuffer();
                continue;
            }
            limparBuffer(); 

            switch (escolha_admin) {
                case 1:
                    criarCliente(&lista_clientes);
                    break;
                case 2:
                    deletarCliente(&lista_clientes);
                    break;
                case 3:
                    criarCriptomoeda(&lista_criptomoedas);
                    break;
                case 4:
                    excluirCriptomoeda(&lista_criptomoedas);
                    break;
                case 5:
                    consultarSaldoAdmin(&lista_clientes);
                    break;
                case 6:
                    consultarExtratoADM(&lista_clientes);
                    break;
                case 7:
                    atualizarCotacao(&bitcoin_price, &ethereum_price, &ripple_price);
                    break;
                case 8:
                    listarCriptomoedas(&lista_criptomoedas);
                    break;
                case 9:
                    printf("Encerrando o programa e salvando os dados...\n");
                    salvarClientes(&lista_clientes, NOME_ARQUIVO_CLIENTES);
                    salvarAdmins(&lista_admins, NOME_ARQUIVO_ADMINS);
                    salvarCriptomoedas(&lista_criptomoedas, NOME_ARQUIVO_CRIPTOMOEDAS);
                    break;
                default:
                    printf("Opção inválida!\n");
            }
        } while (escolha_admin != 9);
    }
    return 0;
}