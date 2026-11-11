
#include <stdio.h>
#include <string.h>


struct Disco {
    int id;                              
    char titulo[50];                     
    char artista[50];                    
    float preco;                         
    int estoque;                        
};

struct Disco discos[50];                 
int totalDiscos = 0;                     
int proximoIdDisco = 1;                  


int discoTemVenda(int idDisco);


void salvarDiscosCSV() {
    int i;
    FILE *arq = fopen("discos.csv", "w");

    if (arq == NULL) {
        printf("Erro ao abrir discos.csv para escrita!\n");
        return;
    }

    fprintf(arq, "id;titulo;artista;preco;estoque\n");    

    for (i = 0; i < totalDiscos; i++) {
        fprintf(arq, "%d;%s;%s;%.2f;%d\n", discos[i].id, discos[i].titulo,
                discos[i].artista, discos[i].preco, discos[i].estoque);
    }

    fclose(arq);
}


void carregarDiscosCSV() {
    char cabecalho[200];
    FILE *arq = fopen("discos.csv", "r");

    if (arq == NULL) {
        printf("discos.csv nao encontrado. Comecando sem discos.\n");
        return;
    }

    fgets(cabecalho, 200, arq);          
    totalDiscos = 0;

   
    while (totalDiscos < 50 &&
           fscanf(arq, "%d;%49[^;];%49[^;];%f;%d\n",
                  &discos[totalDiscos].id, discos[totalDiscos].titulo,
                  discos[totalDiscos].artista, &discos[totalDiscos].preco,
                  &discos[totalDiscos].estoque) == 5) {

        if (discos[totalDiscos].id >= proximoIdDisco) {
            proximoIdDisco = discos[totalDiscos].id + 1;
        }
        totalDiscos++;
    }

    fclose(arq);
    printf("discos.csv: %d discos carregados.\n", totalDiscos);
}


int buscarIndiceDisco(int id) {
    int i;

    for (i = 0; i < totalDiscos; i++) {
        if (discos[i].id == id) {
            return i;
        }
    }
    return -1;
}


void cadastrarDisco() {
    int p = totalDiscos;                

    if (totalDiscos >= 50) {
        printf("Limite de discos atingido!\n");
        return;
    }

    discos[p].id = proximoIdDisco;

    printf("Titulo: ");
    fgets(discos[p].titulo, 50, stdin);
    limparTexto(discos[p].titulo);

    printf("Artista: ");
    fgets(discos[p].artista, 50, stdin);
    limparTexto(discos[p].artista);

    printf("Preco: R$ ");
    scanf("%f", &discos[p].preco);
    getchar();

    printf("Estoque: ");
    scanf("%d", &discos[p].estoque);
    getchar();

    if (discos[p].preco <= 0 || discos[p].estoque < 0) {
        printf("Preco ou estoque invalido! Disco nao cadastrado.\n");
        return;
    }

    totalDiscos++;
    proximoIdDisco++;
    salvarDiscosCSV();                   

    printf("Disco cadastrado com ID %d!\n", discos[p].id);
}


void listarDiscos() {
    int i;

    if (totalDiscos == 0) {
        printf("Nenhum disco cadastrado.\n");
        return;
    }

    printf("\n--- LISTA DE DISCOS ---\n");

    for (i = 0; i < totalDiscos; i++) {
        printf("ID: %d\n", discos[i].id);
        printf("Titulo: %s\n", discos[i].titulo);
        printf("Artista: %s\n", discos[i].artista);
        printf("Preco: R$ %.2f\n", discos[i].preco);
        printf("Estoque: %d\n", discos[i].estoque);
        printf("------------------------\n");
    }
}


void buscarDisco() {
    int i;
    int achou = 0;                       
    char texto[50];

    printf("Digite parte do titulo ou do artista: ");
    fgets(texto, 50, stdin);
    limparTexto(texto);

    for (i = 0; i < totalDiscos; i++) {
        if (strstr(discos[i].titulo, texto) != NULL ||
            strstr(discos[i].artista, texto) != NULL) {
            printf("\nID: %d\n", discos[i].id);
            printf("Titulo: %s\n", discos[i].titulo);
            printf("Artista: %s\n", discos[i].artista);
            printf("Preco: R$ %.2f\n", discos[i].preco);
            printf("Estoque: %d\n", discos[i].estoque);
            achou = 1;
        }
    }

    if (achou == 0) {
        printf("Nenhum disco encontrado.\n");
    }
}


void editarDisco() {
    int i;
    int id;

    printf("Digite o ID do disco que deseja editar: ");
    scanf("%d", &id);
    getchar();

    i = buscarIndiceDisco(id);
    if (i == -1) {
        printf("Disco nao encontrado.\n");
        return;
    }

    printf("Novo titulo: ");
    fgets(discos[i].titulo, 50, stdin);
    limparTexto(discos[i].titulo);

    printf("Novo artista: ");
    fgets(discos[i].artista, 50, stdin);
    limparTexto(discos[i].artista);

    printf("Novo preco: R$ ");
    scanf("%f", &discos[i].preco);
    getchar();

    printf("Novo estoque: ");
    scanf("%d", &discos[i].estoque);
    getchar();

    salvarDiscosCSV();                   
    printf("Disco editado!\n");
}


void removerDisco() {
    int i;
    int id;
    int posicao;

    printf("Digite o ID do disco que deseja remover: ");
    scanf("%d", &id);
    getchar();

    posicao = buscarIndiceDisco(id);
    if (posicao == -1) {
        printf("Disco nao encontrado.\n");
        return;
    }

    if (discoTemVenda(id) == 1) {
        printf("Nao e possivel remover: existem vendas deste disco.\n");
        return;
    }

    
    for (i = posicao; i < totalDiscos - 1; i++) {
        discos[i] = discos[i + 1];
    }

    totalDiscos--;
    salvarDiscosCSV();                   
    printf("Disco removido!\n");
}

// Submenu de discos
// Autor: Breno
void menuDiscos() {
    int opcao;

    do {
        limparTela();
        printf("===== MENU DISCOS =====\n");
        printf("1 - Cadastrar disco\n");
        printf("2 - Listar discos\n");
        printf("3 - Buscar disco\n");
        printf("4 - Editar disco\n");
        printf("5 - Remover disco\n");
        printf("0 - Voltar\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: cadastrarDisco(); pausar(); break;
            case 2: listarDiscos();   pausar(); break;
            case 3: buscarDisco();    pausar(); break;
            case 4: editarDisco();    pausar(); break;
            case 5: removerDisco();   pausar(); break;
            case 0: break;
            default: printf("Opcao invalida!\n"); pausar();
        }

    } while (opcao != 0);
}
