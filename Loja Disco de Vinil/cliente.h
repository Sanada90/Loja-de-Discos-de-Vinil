
#include <stdio.h>
#include <string.h>


struct Cliente {
    int id;                              
    char nome[100];                      
    char telefone[20];                   
    int idade;                           
    int status;                          
};

struct Cliente clientes[50];            
int totalClientes = 0;                   
int proximoIdCliente = 1;                


int clienteTemVenda(int idCliente);


void salvarClientesCSV() {
    int i;
    FILE *arq = fopen("clientes.csv", "w");

    if (arq == NULL) {
        printf("Erro ao abrir clientes.csv para escrita!\n");
        return;
    }

    fprintf(arq, "id;nome;telefone;idade;status\n");       

    for (i = 0; i < totalClientes; i++) {
        fprintf(arq, "%d;%s;%s;%d;%d\n", clientes[i].id, clientes[i].nome,
                clientes[i].telefone, clientes[i].idade, clientes[i].status);
    }

    fclose(arq);
}


void carregarClientesCSV() {
    char cabecalho[200];
    FILE *arq = fopen("clientes.csv", "r");

    if (arq == NULL) {
        printf("clientes.csv nao encontrado. Comecando sem clientes.\n");
        return;
    }

    fgets(cabecalho, 200, arq);          
    totalClientes = 0;

    
    while (totalClientes < 50 &&
           fscanf(arq, "%d;%99[^;];%19[^;];%d;%d\n",
                  &clientes[totalClientes].id, clientes[totalClientes].nome,
                  clientes[totalClientes].telefone, &clientes[totalClientes].idade,
                  &clientes[totalClientes].status) == 5) {

        if (clientes[totalClientes].id >= proximoIdCliente) {
            proximoIdCliente = clientes[totalClientes].id + 1;
        }
        totalClientes++;
    }

    fclose(arq);
    printf("clientes.csv: %d clientes carregados.\n", totalClientes);
}


int buscarIndiceCliente(int id) {
    int i;

    for (i = 0; i < totalClientes; i++) {
        if (clientes[i].id == id) {
            return i;
        }
    }
    return -1;
}


void cadastrarCliente() {
    int p = totalClientes;               

    if (totalClientes >= 50) {
        printf("Limite de clientes atingido!\n");
        return;
    }

    clientes[p].id = proximoIdCliente;

    printf("Nome: ");
    fgets(clientes[p].nome, 100, stdin);
    limparTexto(clientes[p].nome);

    printf("Telefone: ");
    fgets(clientes[p].telefone, 20, stdin);
    limparTexto(clientes[p].telefone);

    printf("Idade: ");
    scanf("%d", &clientes[p].idade);
    getchar();

    if (clientes[p].idade <= 0) {
        printf("Idade invalida! Cliente nao cadastrado.\n");
        return;
    }

    clientes[p].status = 1;              

    totalClientes++;
    proximoIdCliente++;
    salvarClientesCSV();                 

    printf("Cliente cadastrado com ID %d!\n", clientes[p].id);
}


void listarClientes() {
    int i;

    if (totalClientes == 0) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }

    printf("\n--- LISTA DE CLIENTES ---\n");

    for (i = 0; i < totalClientes; i++) {
        printf("ID: %d\n", clientes[i].id);
        printf("Nome: %s\n", clientes[i].nome);
        printf("Telefone: %s\n", clientes[i].telefone);
        printf("Idade: %d\n", clientes[i].idade);

        if (clientes[i].status == 1) {
            printf("Status: Ativo\n");
        } else {
            printf("Status: Inativo\n");
        }

        printf("------------------------\n");
    }
}


void buscarCliente() {
    int i;
    int achou = 0;
    char texto[100];

    printf("Digite parte do nome do cliente: ");
    fgets(texto, 100, stdin);
    limparTexto(texto);

    for (i = 0; i < totalClientes; i++) {
        if (strstr(clientes[i].nome, texto) != NULL) {
            printf("\nID: %d\n", clientes[i].id);
            printf("Nome: %s\n", clientes[i].nome);
            printf("Telefone: %s\n", clientes[i].telefone);
            printf("Idade: %d\n", clientes[i].idade);

            if (clientes[i].status == 1) {
                printf("Status: Ativo\n");
            } else {
                printf("Status: Inativo\n");
            }

            achou = 1;
        }
    }

    if (achou == 0) {
        printf("Nenhum cliente encontrado.\n");
    }
}


void editarCliente() {
    int i;
    int id;

    printf("Digite o ID do cliente que deseja editar: ");
    scanf("%d", &id);
    getchar();

    i = buscarIndiceCliente(id);
    if (i == -1) {
        printf("Cliente nao encontrado.\n");
        return;
    }

    printf("Novo nome: ");
    fgets(clientes[i].nome, 100, stdin);
    limparTexto(clientes[i].nome);

    printf("Novo telefone: ");
    fgets(clientes[i].telefone, 20, stdin);
    limparTexto(clientes[i].telefone);

    printf("Nova idade: ");
    scanf("%d", &clientes[i].idade);
    getchar();

    printf("Novo status (1 - Ativo / 0 - Inativo): ");
    scanf("%d", &clientes[i].status);
    getchar();

    salvarClientesCSV();                 
    printf("Cliente editado!\n");
}



void removerCliente() {
    int i;
    int id;
    int posicao;

    printf("Digite o ID do cliente que deseja remover: ");
    scanf("%d", &id);
    getchar();

    posicao = buscarIndiceCliente(id);
    if (posicao == -1) {
        printf("Cliente nao encontrado.\n");
        return;
    }

    if (clienteTemVenda(id) == 1) {
        printf("Nao e possivel remover: este cliente tem vendas. Edite o status para Inativo.\n");
        return;
    }

    
    for (i = posicao; i < totalClientes - 1; i++) {
        clientes[i] = clientes[i + 1];
    }

    totalClientes--;
    salvarClientesCSV();                 
    printf("Cliente removido!\n");
}


void menuClientes() {
    int opcao;

    do {
        limparTela();
        printf("===== MENU CLIENTES =====\n");
        printf("1 - Cadastrar cliente\n");
        printf("2 - Listar clientes\n");
        printf("3 - Buscar cliente\n");
        printf("4 - Editar cliente\n");
        printf("5 - Remover cliente\n");
        printf("0 - Voltar\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: cadastrarCliente(); pausar(); break;
            case 2: listarClientes();   pausar(); break;
            case 3: buscarCliente();    pausar(); break;
            case 4: editarCliente();    pausar(); break;
            case 5: removerCliente();   pausar(); break;
            case 0: break;
            default: printf("Opcao invalida!\n"); pausar();
        }

    } while (opcao != 0);
}
