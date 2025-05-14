#include "projeto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define NOME_ARQUIVO "clientes.txt"
#define NOME_ARQUIVO_ADMINS "admins.txt" // Novo arquivo para administradores
#define NOME_ARQUIVO_CRIPTOMOEDAS "criptomoedas.txt" // Novo arquivo para criptomoedas
#define MAX_LINE_SIZE 256 // Tamanho máximo de uma linha no arquivo

int carregarClientes(ListaDeCliente *lt, const char *nome) {
    FILE *fp = fopen(nome, "r");
    if (fp == NULL) {
        lt->qtd_cliente = 0;
        return 0;
    }

    lt->qtd_cliente = 0;
    char linha[MAX_LINE_SIZE];

    while (fgets(linha, sizeof(linha), fp) != NULL && lt->qtd_cliente < Total_Cliente) {
        Cliente *cliente = &lt->clientes[lt->qtd_cliente];
        if (sscanf(linha, "%99[^;];%11[^;];%10[^;];%149[^;];%19[^;];%f;%f;%f;%f;%19[^\n]",
                    cliente->nome_completo, cliente->cpf, cliente->dataNascimento,
                    cliente->endereco, cliente->telefone, &cliente->saldo, &cliente->bitcoin, &cliente->ethereum, &cliente->ripple, cliente->senha) == 10) {
            lt->qtd_cliente++;
        }
    }
    fclose(fp);
    return 0;
}

int salvarClientes(ListaDeCliente *lt, const char *nome) {
    FILE *fp = fopen(nome, "w");
    if (fp == NULL) return 1;
    int i;
    for (i = 0; i < lt->qtd_cliente; i++) {
        fprintf(fp, "%s;%s;%s;%s;%s;%.2f;%.2f;%.2f;%.2f;%s\n",
                lt->clientes[i].nome_completo, lt->clientes[i].cpf, lt->clientes[i].dataNascimento,
                lt->clientes[i].endereco, lt->clientes[i].telefone, lt->clientes[i].saldo, lt->clientes[i].bitcoin, lt->clientes[i].ethereum, lt->clientes[i].ripple, lt->clientes[i].senha);
    }
    fclose(fp);
    return 0;
}


int criarCliente(ListaDeCliente *lt) {
    if (lt->qtd_cliente >= Total_Cliente) {
        printf("Limite de clientes atingido!\n");
        return 1;
    }

    Cliente *t = &lt->clientes[lt->qtd_cliente];

    printf("Escreva o nome completo:\n");
    scanf(" %[^\n]", t->nome_completo);

    printf("Escreva o seu [CPF]:\n");
    scanf(" %11s", t->cpf);

    printf("Escreva a data de nascimento (DD/MM/AAAA):\n");
    scanf(" %10s", t->dataNascimento);

    printf("Escreva o endereço em que reside:\n");
    scanf(" %[^\n]", t->endereco);

    printf("Escreva o seu número de telefone:\n");
    scanf(" %19s", t->telefone);

    printf("Escreva sua senha:\n");
    scanf(" %19s", t->senha);

    t->saldo = 0.0;
    t->bitcoin = 0.0;
    t->ethereum = 0.0;
    t->ripple = 0.0;

    lt->qtd_cliente++;
    salvarClientes(lt, NOME_ARQUIVO);

    printf("Cliente cadastrado com sucesso!\n");
    return 0;
}

int deletarCliente(ListaDeCliente *lt) {
    if (lt->qtd_cliente == 0) {
        printf("Nenhum cliente cadastrado para deletar!\n");
        return 1;
    }

    char cpf[12];
    printf("Escreva o CPF do cliente que deseja deletar:\n");
    scanf(" %11s", cpf);

    int pos = -1;
    int i;
    for (i = 0; i < lt->qtd_cliente; i++) {
        if (strcmp(lt->clientes[i].cpf, cpf) == 0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("Cliente não encontrado!\n");
        return 2;
    }

    for (i = pos; i < lt->qtd_cliente - 1; i++) {
        lt->clientes[i] = lt->clientes[i + 1];
    }

    lt->qtd_cliente--;
    salvarClientes(lt, NOME_ARQUIVO); // Salvar a lista atualizada

    printf("Cliente deletado com sucesso!\n");
    return 0;
}

int listarCliente(ListaDeCliente *lt) {
    if (lt->qtd_cliente == 0) {
        printf("Nenhum cliente cadastrado.\n");
        return 0;
    }
	
	int i;
    for (i = 0; i < lt->qtd_cliente; i++) {
        printf("\tCliente %d:\n", i + 1);
        printf("-----------------------------\n");
        printf("Nome: %s\n", lt->clientes[i].nome_completo);
        printf("CPF: %s\n", lt->clientes[i].cpf);
        printf("Data de Nascimento: %s\n", lt->clientes[i].dataNascimento);
        printf("Endereço: %s\n", lt->clientes[i].endereco);
        printf("Telefone: %s\n", lt->clientes[i].telefone);
        printf("Saldo em reais: R$ %.2f\n", lt->clientes[i].saldo);
        printf("Criptomoedas:\n");
        printf("  Bitcoin: %.6f BTC\n", lt->clientes[i].bitcoin);
        printf("  Ethereum: %.6f ETH\n", lt->clientes[i].ethereum);
        printf("  Ripple: %.6f XRP\n", lt->clientes[i].ripple);
        printf("-----------------------------\n");
    }
    return 1;
}


int consultarSaldo(ListaDeCliente *lt) {
    char cpf[12];
    printf("Digite o CPF do cliente para consultar o saldo:\n");
    scanf(" %11s", cpf);

    int i;
    for (i = 0; i < lt->qtd_cliente; i++) {
        if (strcmp(lt->clientes[i].cpf, cpf) == 0) {
            printf("Saldo do cliente %s: %.2f\n", lt->clientes[i].nome_completo, lt->clientes[i].saldo);
            return 0;
        }
    }

    printf("Cliente não encontrado.\n");
    return 1;
}

int depositar(ListaDeCliente *lt) {
    char cpf[12];
    float valor;

    printf("Digite o CPF do cliente para depósito:\n");
    scanf(" %11s", cpf);
    int i;
    for (i = 0; i < lt->qtd_cliente; i++) {
        if (strcmp(lt->clientes[i].cpf, cpf) == 0) {
            printf("Digite o valor a ser depositado:\n");
            scanf("%f", &valor);

            if (valor > 0) {
                lt->clientes[i].saldo += valor;
                salvarClientes(lt, NOME_ARQUIVO);
                printf("Depósito de %.2f realizado com sucesso.\n", valor);
                return 0;
            } else {
                printf("Valor inválido para depósito.\n");
                return 1;
            }
        }
    }

    printf("Cliente não encontrado.\n");
    return 1;
}

int sacar(ListaDeCliente *lt) {
    char cpf[12];
    float valor;

    printf("Digite o CPF do cliente para saque:\n");
    scanf(" %11s", cpf);
    int i;
    for (i = 0; i < lt->qtd_cliente; i++) {
        if (strcmp(lt->clientes[i].cpf, cpf) == 0) {
            printf("Digite o valor a ser sacado:\n");
            scanf("%f", &valor);

            if (valor > 0 && valor <= lt->clientes[i].saldo) {
                lt->clientes[i].saldo -= valor;
                salvarClientes(lt, NOME_ARQUIVO);
                printf("Saque de %.2f realizado com sucesso.\n", valor);
                return 0;
            } else if (valor <= 0) {
                printf("Valor inválido para saque.\n");
                return 1;
            } else {
                printf("Saldo insuficiente.\n");
                return 1;
            }
        }
    }

    printf("Cliente não encontrado.\n");
    return 1;
}

int comprarCripto(ListaDeCliente *lt, ListaDeCriptomoedas *lc) {
    char cpf[12];
    float valor_reais;

    printf("Digite o CPF do cliente:\n");
    scanf(" %11s", cpf);

    int i;
    for (i = 0; i < lt->qtd_cliente; i++) {
        if (strcmp(lt->clientes[i].cpf, cpf) == 0) {
            if (lc->qtd_criptomoedas == 0) {
                printf("Nenhuma criptomoeda disponível para compra.\n");
                return 1;
            }
			
			int j;
            printf("Criptomoedas disponíveis:\n");
            for (j = 0; j < lc->qtd_criptomoedas; j++) {
                printf("%d. %s (%s) - Preço: %.2f\n", j + 1,
                       lc->criptomoedas[j].nome,
                       lc->criptomoedas[j].simbolo,
                       lc->criptomoedas[j].preco);
            }

            int escolha;
            printf("Escolha o número da criptomoeda que deseja comprar:\n");
            scanf("%d", &escolha);
            escolha--;

            if (escolha < 0 || escolha >= lc->qtd_criptomoedas) {
                printf("Opção inválida.\n");
                return 1;
            }

            printf("Digite o valor em reais para investir:\n");
            scanf("%f", &valor_reais);

            if (valor_reais <= 0 || valor_reais > lt->clientes[i].saldo) {
                printf("Valor inválido ou saldo insuficiente.\n");
                return 1;
            }

            double qtd_comprada = valor_reais / lc->criptomoedas[escolha].preco;
            lt->clientes[i].saldo -= valor_reais;

            // Atualizar apenas se for BTC, ETH ou XRP
            if (strcmp(lc->criptomoedas[escolha].simbolo, "BTC") == 0) {
                lt->clientes[i].bitcoin += qtd_comprada;
            } else if (strcmp(lc->criptomoedas[escolha].simbolo, "ETH") == 0) {
                lt->clientes[i].ethereum += qtd_comprada;
            } else if (strcmp(lc->criptomoedas[escolha].simbolo, "XRP") == 0) {
                lt->clientes[i].ripple += qtd_comprada;
            }

            salvarClientes(lt, NOME_ARQUIVO);

            // Registrar no extrato
            char nome_arquivo[50];
            snprintf(nome_arquivo, sizeof(nome_arquivo), "extrato_%s.txt", cpf);
            FILE *fp = fopen(nome_arquivo, "a");
            if (fp != NULL) {
                time_t agora = time(NULL);
                fprintf(fp, "[%s] Compra: %.6f de %s por R$%.2f\n",
                        strtok(ctime(&agora), "\n"),
                        qtd_comprada,
                        lc->criptomoedas[escolha].simbolo,
                        valor_reais);
                fclose(fp);
            }

            printf("Você comprou %.6f de %s.\n", qtd_comprada, lc->criptomoedas[escolha].simbolo);
            return 0;
        }
    }

    printf("Cliente não encontrado.\n");
    return 1;
}




int venderCripto(ListaDeCliente *lt, ListaDeCriptomoedas *lc) {
    char cpf[12];
    float quantidade;

    printf("Digite o CPF do cliente:\n");
    scanf(" %11s", cpf);

    int i;
    for (i = 0; i < lt->qtd_cliente; i++) {
        if (strcmp(lt->clientes[i].cpf, cpf) == 0) {
            if (lc->qtd_criptomoedas == 0) {
                printf("Nenhuma criptomoeda disponível.\n");
                return 1;
            }
	
			int j;
            printf("Criptomoedas disponíveis:\n");
            for (j = 0; j < lc->qtd_criptomoedas; j++) {
                printf("%d. %s (%s) - Preço atual: %.2f\n", j + 1,
                       lc->criptomoedas[j].nome,
                       lc->criptomoedas[j].simbolo,
                       lc->criptomoedas[j].preco);
            }

            int escolha;
            printf("Escolha o número da criptomoeda que deseja vender:\n");
            scanf("%d", &escolha);
            escolha--;

            if (escolha < 0 || escolha >= lc->qtd_criptomoedas) {
                printf("Opção inválida.\n");
                return 1;
            }

            printf("Digite a quantidade a ser vendida:\n");
            scanf("%f", &quantidade);

            Criptomoeda *cr = &lc->criptomoedas[escolha];
            float *saldoCripto = NULL;

            if (strcmp(cr->simbolo, "BTC") == 0) {
                saldoCripto = &lt->clientes[i].bitcoin;
            } else if (strcmp(cr->simbolo, "ETH") == 0) {
                saldoCripto = &lt->clientes[i].ethereum;
            } else if (strcmp(cr->simbolo, "XRP") == 0) {
                saldoCripto = &lt->clientes[i].ripple;
            } else {
                printf("Venda de outras criptomoedas ainda não suportada.\n");
                return 1;
            }

            if (quantidade > 0 && quantidade <= *saldoCripto) {
                float taxa = 0.97; // exemplo de taxa de 3%
                float valor_obtido = quantidade * cr->preco * taxa;
                *saldoCripto -= quantidade;
                lt->clientes[i].saldo += valor_obtido;
                salvarClientes(lt, NOME_ARQUIVO);
                printf("Venda de %.4f %s realizada. Valor recebido: R$ %.2f\n", quantidade, cr->simbolo, valor_obtido);
                return 0;
            } else {
                printf("Quantidade inválida ou saldo insuficiente.\n");
                return 1;
            }
        }
    }

    printf("Cliente não encontrado.\n");
    return 1;
}


size_t write_data(void *ptr, size_t size, size_t nmemb, char **data) {
    size_t total_size = size * nmemb;
    *data = realloc(*data, total_size + 1);
    if (*data == NULL) {
        return 0;
    }
    memcpy(*data, ptr, total_size);
    (*data)[total_size] = '\0';
    return total_size;
}

// Função para gerar um número aleatório entre -5% e +5%
double gerarVariacao() {
    return ((double)rand() / RAND_MAX) * 0.10 - 0.05; // Gera um número entre -0.05 e 0.05
}

int atualizarCotacao(double *bitcoin_price, double *ethereum_price, double *ripple_price) {
    srand(time(NULL));

    *bitcoin_price += *bitcoin_price * gerarVariacao();
    *ethereum_price += *ethereum_price * gerarVariacao();
    *ripple_price += *ripple_price * gerarVariacao();

    printf("Cotação Bitcoin (variada): USD %.2f\n", *bitcoin_price);
    printf("Cotação Ethereum (variada): USD %.2f\n", *ethereum_price);
    printf("Cotação Ripple (variada): USD %.2f\n", *ripple_price);

    return 0;
}

// Função para acessar a conta com CPF e senha.
int loginCliente(ListaDeCliente *lt) {
    char cpf_digitado[12];
    char senha_digitada[20];

    while (1) { // Loop infinito para tentativas de login
        printf("Digite o CPF (ou 'sair' para encerrar): ");
        if (scanf(" %11s", cpf_digitado) != 1) {
            printf("Entrada inválida.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        if (strcmp(cpf_digitado, "sair") == 0) {
            return 1; // Encerra o programa se o usuário digitar "sair"
        }

        printf("Digite a senha: ");
        if (scanf(" %19s", senha_digitada) != 1) {
            printf("Entrada inválida.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        int i;
        for (i = 0; i < lt->qtd_cliente; i++) {
            if (strcmp(lt->clientes[i].cpf, cpf_digitado) == 0 && strcmp(lt->clientes[i].senha, senha_digitada) == 0) {
                printf("Login realizado com sucesso!\n");
                return 0; // Login bem-sucedido
            }
        }

        printf("CPF ou senha incorretos. Tente novamente.\n");
    }
}

int loginAdmin(ListaDeAdm *la) {
    char cpf_digitado[12];
    char senha_digitada[20];

    while (1) { // Loop infinito para tentativas de login
        printf("Digite o CPF (ou 'sair' para encerrar): ");
        if (scanf(" %11s", cpf_digitado) != 1) {
            printf("Entrada inválida.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        if (strcmp(cpf_digitado, "sair") == 0) {
            return 1; // Encerra o programa se o usuário digitar "sair"
        }

        printf("Digite a senha: ");
        if (scanf(" %19s", senha_digitada) != 1) {
            printf("Entrada inválida.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        int i;
        for (i = 0; i < la->qtd_adm; i++) {
            if (strcmp(la->adm[i].cpf, cpf_digitado) == 0 && strcmp(la->adm[i].senha, senha_digitada) == 0) {
                printf("Login realizado com sucesso!\n");
                return 0; // Login bem-sucedido
            }
        }

        printf("CPF ou senha incorretos. Tente novamente.\n");
    }
}

int criarAdmin(ListaDeAdm *la) {
    if (la->qtd_adm >= TOTAL_ADM) {
        printf("Limite de clientes atingido!\n");
        return 1;
    }

    Adm *t = &la->adm[la->qtd_adm];

    printf("Escreva o nome completo:\n");
    scanf(" %[^\n]", t->nome_completo);

    printf("Escreva o seu [CPF]:\n");
    scanf(" %11s", t->cpf);

    printf("Escreva a data de nascimento (DD/MM/AAAA):\n");
    scanf(" %10s", t->dataNascimento);

    printf("Escreva o endereço em que reside:\n");
    scanf(" %[^\n]", t->endereco);

    printf("Escreva o seu número de telefone:\n");
    scanf(" %19s", t->telefone);

    printf("Escreva sua senha:\n");
    scanf(" %19s", t->senha);

    la->qtd_adm++;
    salvarAdmins(la, NOME_ARQUIVO_ADMINS);

    printf("Adm cadastrado com sucesso!\n");
    return 0;
}

int carregarAdmins(ListaDeAdm *la, const char *nome) {
    FILE *fp = fopen(nome, "r");
    if (fp == NULL) {
        la->qtd_adm = 0;
        return 0;
    }

    la->qtd_adm = 0;
    char linha[MAX_LINE_SIZE];

    while (fgets(linha, sizeof(linha), fp) != NULL && la->qtd_adm < TOTAL_ADM) {
        Adm *adm = &la->adm[la->qtd_adm];
        if (sscanf(linha, "%99[^;];%11[^;];%10[^;];%149[^;];%19[^;];%19[^\n]",
                    adm->nome_completo, adm->cpf, adm->dataNascimento,
                    adm->endereco, adm->telefone, adm->senha) == 6) {
            la->qtd_adm++;
        }
    }
    fclose(fp);
    return 0;
}

int salvarAdmins(ListaDeAdm *la, const char *nome) {
    FILE *fp = fopen(nome, "w");
    if (fp == NULL) return 1;
    int i;
    for (i = 0; i < la->qtd_adm; i++) {
        fprintf(fp, "%s;%s;%s;%s;%s;%s\n",
                la->adm[i].nome_completo, la->adm[i].cpf, la->adm[i].dataNascimento,
                la->adm[i].endereco, la->adm[i].telefone, la->adm[i].senha);
    }
    fclose(fp);
    return 0;
}

int criarCriptomoeda(ListaDeCriptomoedas *lc) {
    if (lc->qtd_criptomoedas >= TOTAL_CRIPTOMOEDAS) {
        printf("Limite de criptomoedas atingido! Não é possível adicionar mais.\n");
        return 1; // Retorna 1 para indicar erro (limite atingido)
    }

    // Ponteiro para a próxima criptomoeda a ser adicionada na lista
    Criptomoeda *nova_cripto = &lc->criptomoedas[lc->qtd_criptomoedas];

    printf("Escreva o nome da criptomoeda (ex: Bitcoin):\n");
    scanf(" %49[^\n]", nova_cripto->nome); // Lê até 49 caracteres para nome[50]
    while (getchar() != '\n'); // Limpa o buffer do teclado para a próxima leitura

    printf("Escreva o símbolo da criptomoeda (ex: BTC):\n");
    scanf(" %9s", nova_cripto->simbolo); // Lê até 9 caracteres para simbolo[10]
    while (getchar() != '\n'); // Limpa o buffer do teclado

    printf("Escreva o preço da criptomoeda (ex: 60000.50):\n");
    // CORREÇÃO PRINCIPAL: Usar %lf para ler um double
    if (scanf("%lf", &nova_cripto->preco) != 1) {
        printf("Erro ao ler o preço. Por favor, insira um número válido.\n");
        // Opcional: decidir se quer retornar erro aqui ou tentar de novo
        // Por enquanto, o preço pode ficar com valor incorreto se a leitura falhar.
        // Uma abordagem mais robusta seria validar a entrada.
        nova_cripto->preco = 0.0; // Define um valor padrão em caso de falha na leitura.
    }
    while (getchar() != '\n'); // Limpa o buffer do teclado

    lc->qtd_criptomoedas++; // Incrementa a quantidade de criptomoedas na lista

    // Salva toda a lista de criptomoedas no arquivo
    if (salvarCriptomoedas(lc, NOME_ARQUIVO_CRIPTOMOEDAS) == 0) {
        printf("Criptomoeda '%s' cadastrada e salva com sucesso!\n", nova_cripto->nome);
    } else {
        printf("Criptomoeda '%s' cadastrada, mas houve um erro ao salvar no arquivo.\n", nova_cripto->nome);
        // Mesmo com erro ao salvar, a criptomoeda foi adicionada à lista em memória.
        // Poderia-se decrementar qtd_criptomoedas aqui se o salvamento for crítico.
    }

    return 0; // Retorna 0 para indicar sucesso na adição à lista (mesmo que salvar falhe)
}

int excluirCriptomoeda(ListaDeCriptomoedas *lc){
    if (lc->qtd_criptomoedas == 0) {
        printf("Nenhuma criptomoeda cadastrada para deletar!\n");
        return 1;
    }

    char nome[100];
    printf("Escreva o nome da criptomoeda que deseja deletar:\n");
    scanf(" %[^\n]", nome);

    int pos = -1;
    int i;
    for (i = 0; i < lc->qtd_criptomoedas; i++) {
        if (strcmp(lc->criptomoedas[i].nome, nome) == 0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("Criptomoeda não encontrada!\n");
        return 2;
    }

    for (i = pos; i < lc->qtd_criptomoedas - 1; i++) {
        lc->criptomoedas[i] = lc->criptomoedas[i + 1];
    }

    lc->qtd_criptomoedas--;
    salvarCriptomoedas(lc, NOME_ARQUIVO_CRIPTOMOEDAS); // Salvar a lista atualizada

    printf("Criptomoeda deletada com sucesso!\n");
    return 0;
}

void listarCriptomoedas(ListaDeCriptomoedas *lc){
    if (lc->qtd_criptomoedas == 0) {
        printf("Nenhuma criptomoeda cadastrada.\n");
        return;
    }

    printf("Lista de Criptomoedas:\n");
    int i;
    for (i = 0; i < lc->qtd_criptomoedas; i++) {
        printf("\nCriptomoeda %d:\n", i + 1);
        printf("Nome: %s\n", lc->criptomoedas[i].nome);
        printf("Símbolo: %s\n", lc->criptomoedas[i].simbolo);
        printf("Preço: %.2f\n", lc->criptomoedas[i].preco);
    }
}

int carregarCriptomoedas(ListaDeCriptomoedas *lc, const char *nome_arquivo) {
    FILE *fp = fopen(nome_arquivo, "r");
    if (fp == NULL) {
        // Se o arquivo não existe (primeira execução, por exemplo),
        // não é necessariamente um erro crítico aqui. A lista fica vazia.
        // Se quiser tratar como erro, pode adicionar: perror("Erro ao abrir arquivo de criptomoedas");
        lc->qtd_criptomoedas = 0;
        return 0; // Retorna 0, indicando que o arquivo pode não existir ou não foi aberto.
    }

    lc->qtd_criptomoedas = 0; // Reseta a quantidade antes de carregar do arquivo
    char linha[MAX_LINE_SIZE];

    while (fgets(linha, sizeof(linha), fp) != NULL && lc->qtd_criptomoedas < TOTAL_CRIPTOMOEDAS) {
        // Remove o \n do final da linha, se existir, pois fgets o captura
        linha[strcspn(linha, "\n")] = '\0';

        // Verifica se a linha não está vazia após remover o \n
        if (strlen(linha) == 0) {
            continue; // Pula linhas em branco
        }

        Criptomoeda *cripto_atual = &lc->criptomoedas[lc->qtd_criptomoedas];

        // Tenta ler os três campos: nome (string), símbolo (string), preço (double)
        // Usamos %49[^;] para ler até 49 chars para cripto_atual->nome (tamanho 50)
        // Usamos %9[^;] para ler até 9 chars para cripto_atual->simbolo (tamanho 10)
        // Usamos %lf para ler um double para cripto_atual->preco
        int campos_lidos = sscanf(linha, "%49[^;];%9[^;];%lf",
                                  cripto_atual->nome,
                                  cripto_atual->simbolo,
                                  &cripto_atual->preco);

        if (campos_lidos == 3) {
            // Todos os campos foram lidos com sucesso
            lc->qtd_criptomoedas++;
        } else {
            // A linha não corresponde ao formato esperado. Pode ser útil logar isso.
            printf("Aviso: Linha ignorada ou mal formatada no arquivo de criptomoedas: '%s'\n", linha);
            printf("Campos lidos na linha problemática: %d\n", campos_lidos);
        }
    }

    fclose(fp);
    return 0; // Operação de carregamento concluída (pode ter carregado 0 ou mais itens)
}

int salvarCriptomoedas(ListaDeCriptomoedas *lc, const char *nome) {
    FILE *fp = fopen(nome, "w");
    if (fp == NULL) {
        perror("Erro ao abrir arquivo para salvar criptomoedas");
        return 1; // Erro
    }
    int i;
    for (i = 0; i < lc->qtd_criptomoedas; i++) {
        fprintf(fp, "%s;%s;%f\n", // %f é CORRETO para imprimir double com fprintf
                lc->criptomoedas[i].nome,
                lc->criptomoedas[i].simbolo,
                lc->criptomoedas[i].preco);
    }
    fclose(fp);
    return 0; // Sucesso
}

void exibirMenu() {
    printf("\nMenu Cliente:\n");
    printf("1. Criar Cliente\n");
    printf("2. Deletar Cliente\n");
    printf("3. Listar Cliente\n");
    printf("4. Consultar Saldo\n");
    printf("5. Depositar\n");
    printf("6. Sacar\n");
    printf("7. Compra de criptomoeda\n");
    printf("8. Venda de criptomoeda\n");
    printf("9. Atualizar cotação\n");
    printf("10. Consultar Extrato\n");
    printf("11. Voltar\n"); 
}


void exibirMenuAdmin() {
    printf("\nMenu Administrador:\n");
    printf("1. Criar Cliente\n");
    printf("2. Deletar Cliente\n");
    printf("3. Cadastrar Criptomoeda\n");
    printf("4. Excluir Criptomoeda\n");
    printf("5. Consultar Saldo\n");
    printf("6. Consultar Extrato\n");
    printf("7. Atualizar cotação\n");
    printf("8. Listar Criptomoeda\n");
    printf("9. Voltar\n");
}

int consultarExtrato(ListaDeCliente *lt) {
    char cpf[12];
    printf("Digite o CPF do cliente para exibir o extrato:\n");
    if (scanf(" %11s", cpf) != 1) {
        printf("Entrada inválida.\n");
        return 1;
    }
    getchar(); // Limpar o buffer do teclado

    // Gerar o nome do arquivo do extrato baseado no CPF
    char nome_arquivo[50];
    snprintf(nome_arquivo, sizeof(nome_arquivo), "extrato_%s.txt", cpf);

    // Tentar abrir o arquivo de extrato para leitura
    FILE *fp = fopen(nome_arquivo, "r");
    if (fp == NULL) {
        printf("Nenhuma movimentação encontrada para este cliente.\n");
        return 1;
    }

    // Ler e exibir as linhas do arquivo
    char linha[256];
    printf("\nExtrato de movimentações:\n");
    while (fgets(linha, sizeof(linha), fp)) {
        printf("%s", linha);
    }

    fclose(fp);
    return 0;  // Retorna 0 para indicar que a função foi executada com sucesso
}


void adicionarCriptosPadrao(ListaDeCriptomoedas *lc) {
    char *nomes[] = {"Bitcoin", "Ethereum", "Ripple"};
    char *simbolos[] = {"BTC", "ETH", "XRP"};
    double precos[] = {30000.0, 2000.0, 0.5};
	
	int i;
    for (i = 0; i < 3; i++) {
        int existe = 0;
        int j;
        for (j = 0; j < lc->qtd_criptomoedas; j++) {
            if (strcmp(lc->criptomoedas[j].simbolo, simbolos[i]) == 0) {
                existe = 1;
                break;
            }
        }

        if (!existe) {
            if (lc->qtd_criptomoedas >= TOTAL_CRIPTOMOEDAS) {
                printf("Limite de criptomoedas atingido. Não foi possível adicionar %s.\n", nomes[i]);
                continue;
            }

            Criptomoeda *c = &lc->criptomoedas[lc->qtd_criptomoedas];
            strcpy(c->nome, nomes[i]);
            strcpy(c->simbolo, simbolos[i]);
            c->preco = precos[i];
            lc->qtd_criptomoedas++;
        }
    }

    salvarCriptomoedas(lc, NOME_ARQUIVO_CRIPTOMOEDAS);
}

int consultarExtratoADM(ListaDeCliente *lt) {
    printf("\n--- Consultar Extrato ---\n");
    if (lt->qtd_cliente == 0) {
        printf("Nenhum cliente cadastrado.\n");
        return 1;
    }
	
	int i;
    printf("Lista de CPFs cadastrados:\n");
    for (i = 0; i < lt->qtd_cliente; i++) {
        printf("%d. %s\n", i + 1, lt->clientes[i].cpf);
    }

    int escolha;
    printf("Escolha o número do cliente para ver o extrato (0 para cancelar): ");
    if (scanf("%d", &escolha) != 1 || escolha < 0 || escolha > lt->qtd_cliente) {
        printf("Entrada inválida.\n");
        return 1;
    }

    if (escolha == 0) {
        printf("Cancelando...\n");
        return 0;
    }

    // Selecionar o CPF do cliente
    Cliente *cliente = &lt->clientes[escolha - 1];
    char nome_arquivo[50];
    snprintf(nome_arquivo, sizeof(nome_arquivo), "extrato_%s.txt", cliente->cpf);

    FILE *fp = fopen(nome_arquivo, "r");
    if (fp == NULL) {
        printf("Nenhuma movimentação encontrada para o cliente %s.\n", cliente->cpf);
        return 1;
    }

    char linha[256];
    printf("\nExtrato de movimentações para o CPF %s:\n", cliente->cpf);
    while (fgets(linha, sizeof(linha), fp)) {
        printf("%s", linha);
    }
    fclose(fp);

    return 0;
}

int consultarSaldoAdmin(ListaDeCliente *lt) {
    printf("\n--- Consultar Saldo de Cliente ---\n");

    if (lt->qtd_cliente == 0) {
        printf("Nenhum cliente cadastrado.\n");
        return 1;
    }
	
	int i;
    printf("Lista de CPFs cadastrados:\n");
    for (i = 0; i < lt->qtd_cliente; i++) {
        printf("%d. %s\n", i + 1, lt->clientes[i].cpf);
    }

    int escolha;
    printf("Escolha o número do cliente para ver o saldo (0 para cancelar): ");
    if (scanf("%d", &escolha) != 1 || escolha < 0 || escolha > lt->qtd_cliente) {
        printf("Entrada inválida.\n");
        return 1;
    }

    if (escolha == 0) {
        printf("Cancelando...\n");
        return 0;
    }

    Cliente *cliente = &lt->clientes[escolha - 1];

    printf("\nInformações financeiras do cliente %s:\n", cliente->cpf);
    printf("Nome: %s\n", cliente->nome_completo);
    printf("Saldo em conta: R$ %.2f\n", cliente->saldo);
    printf("Bitcoin: %.6f BTC\n", cliente->bitcoin);
    printf("Ethereum: %.6f ETH\n", cliente->ethereum);
    printf("Ripple: %.6f XRP\n", cliente->ripple);

    return 0;
}