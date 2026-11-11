#include <stdio.h>
#include <string.h>

struct Venda {
    int id;                              
    int idCliente;                     
    int idDisco;                         
    int quantidade;                      
    float valorTotal;                    
};

struct Venda vendas[50];                 
int totalVendas = 0;                     
int proximoIdVenda = 1;                  


int discoTemVenda(int idDisco) {
    int i;

    for (i = 0; i < totalVendas; i++) {
        if (vendas[i].idDisco == idDisco) {
            return 1;
        }
    }
    return 0;
}


int clienteTemVenda(int idCliente) {
    int i;

    for (i = 0; i < totalVendas; i++) {
        if (vendas[i].idCliente == idCliente) {
            return 1;
        }
    }
    return 0;
}


void salvarVendasCSV() {
    int i;
    FILE *arq = fopen("vendas.csv", "w");

    if (arq == NULL) {
        printf("Erro ao abrir vendas.csv para escrita!\n");
        return;
    }

    fprintf(arq, "id;idCliente;idDisco;quantidade;valorTotal\n");  

    for (i = 0; i < totalVendas; i++) {
        fprintf(arq, "%d;%d;%d;%d;%.2f\n", vendas[i].id, vendas[i].idCliente,
                vendas[i].idDisco, vendas[i].quantidade, vendas[i].valorTotal);
    }

    fclose(arq);
}


void carregarVendasCSV() {
    char cabecalho[200];
    FILE *arq = fopen("vendas.csv", "r");

    if (arq == NULL) {
        printf("vendas.csv nao encontrado. Comecando sem vendas.\n");
        return;
    }

    fgets(cabecalho, 200, arq);          // le e joga fora a linha de cabecalho
    totalVendas = 0;

    while (totalVendas < 50 &&
           fscanf(arq, "%d;%d;%d;%d;%f\n",
                  &vendas[totalVendas].id, &vendas[totalVendas].idCliente,
                  &vendas[totalVendas].idDisco, &vendas[totalVendas].quantidade,
                  &vendas[totalVendas].valorTotal) == 5) {

        if (vendas[totalVendas].id >= proximoIdVenda) {
            proximoIdVenda = vendas[totalVendas].id + 1;
        }
        totalVendas++;
    }

    fclose(arq);
    printf("vendas.csv: %d vendas carregadas.\n", totalVendas);
}


int buscarIndiceVenda(int id) {
    int i;

    for (i = 0; i < totalVendas; i++) {
        if (vendas[i].id == id) {
            return i;
        }
    }
    return -1;
}

void mostrarVenda(int i) {
    int c = buscarIndiceCliente(vendas[i].idCliente);
    int d = buscarIndiceDisco(vendas[i].idDisco);

    printf("ID da venda: %d\n", vendas[i].id);

    if (c != -1) {
        printf("Cliente: %s (ID %d)\n", clientes[c].nome, vendas[i].idCliente);
    } else {
        printf("Cliente: ID %d (nao encontrado)\n", vendas[i].idCliente);
    }

    if (d != -1) {
        printf("Disco: %s - %s (ID %d)\n", discos[d].titulo, discos[d].artista, vendas[i].idDisco);
    } else {
        printf("Disco: ID %d (nao encontrado)\n", vendas[i].idDisco);
    }

    printf("Quantidade: %d\n", vendas[i].quantidade);
    printf("Valor total: R$ %.2f\n", vendas[i].valorTotal);
}


void cadastrarVenda() {
    int p = totalVendas;                 
    int idCliente;
    int idDisco;
    int quantidade;
    int c;                               
    int d;                              

    if (totalVendas >= 50) {
        printf("Limite de vendas atingido!\n");
        return;
    }

    printf("ID do cliente: ");
    scanf("%d", &idCliente);
    getchar();

    c = buscarIndiceCliente(idCliente);
    if (c == -1) {
        printf("Cliente nao encontrado. Venda cancelada.\n");
        return;
    }
    if (clientes[c].status == 0) {
        printf("Cliente inativo. Venda cancelada.\n");
        return;
    }
    printf("Cliente: %s\n", clientes[c].nome);

    printf("ID do disco: ");
    scanf("%d", &idDisco);
    getchar();

    d = buscarIndiceDisco(idDisco);
    if (d == -1) {
        printf("Disco nao encontrado. Venda cancelada.\n");
        return;
    }
    printf("Disco: %s - %s | R$ %.2f | estoque: %d\n", discos[d].titulo,
           discos[d].artista, discos[d].preco, discos[d].estoque);

    printf("Quantidade: ");
    scanf("%d", &quantidade);
    getchar();

    if (quantidade <= 0) {
        printf("Quantidade invalida. Venda cancelada.\n");
        return;
    }
    if (quantidade > discos[d].estoque) {
        printf("Estoque insuficiente! Disponivel: %d. Venda cancelada.\n", discos[d].estoque);
        return;
    }

    vendas[p].id = proximoIdVenda;
    vendas[p].idCliente = idCliente;
    vendas[p].idDisco = idDisco;
    vendas[p].quantidade = quantidade;
    vendas[p].valorTotal = discos[d].preco * quantidade;     

    discos[d].estoque = discos[d].estoque - quantidade;     

    totalVendas++;
    proximoIdVenda++;

    salvarVendasCSV();                  
    salvarDiscosCSV();                   

    printf("Venda registrada com ID %d! Total: R$ %.2f\n", vendas[p].id, vendas[p].valorTotal);
}


void listarVendas() {
    int i;

    if (totalVendas == 0) {
        printf("Nenhuma venda cadastrada.\n");
        return;
    }

    printf("\n--- LISTA DE VENDAS ---\n");

    for (i = 0; i < totalVendas; i++) {
        mostrarVenda(i);
        printf("------------------------\n");
    }
}


void buscarVenda() {
    int i;
    int c;
    int achou = 0;
    char texto[100];

    printf("Digite parte do nome do cliente: ");
    fgets(texto, 100, stdin);
    limparTexto(texto);

    for (i = 0; i < totalVendas; i++) {
        c = buscarIndiceCliente(vendas[i].idCliente);

        if (c != -1 && strstr(clientes[c].nome, texto) != NULL) {
            printf("\n");
            mostrarVenda(i);
            achou = 1;
        }
    }

    if (achou == 0) {
        printf("Nenhuma venda encontrada para esse cliente.\n");
    }
}


void editarVenda() {
    int i;
    int d;
    int id;
    int novaQuantidade;
    int diferenca;
    float precoUnitario;

    printf("Digite o ID da venda que deseja editar: ");
    scanf("%d", &id);
    getchar();

    i = buscarIndiceVenda(id);
    if (i == -1) {
        printf("Venda nao encontrada.\n");
        return;
    }

    d = buscarIndiceDisco(vendas[i].idDisco);
    if (d == -1) {
        printf("Disco desta venda nao encontrado. Nao e possivel editar.\n");
        return;
    }

    printf("Quantidade atual: %d\n", vendas[i].quantidade);
    printf("Nova quantidade: ");
    scanf("%d", &novaQuantidade);
    getchar();

    if (novaQuantidade <= 0) {
        printf("Quantidade invalida.\n");
        return;
    }

    diferenca = novaQuantidade - vendas[i].quantidade;   

    if (diferenca > discos[d].estoque) {
        printf("Estoque insuficiente! Disponivel: %d\n", discos[d].estoque);
        return;
    }

    
    precoUnitario = vendas[i].valorTotal / vendas[i].quantidade;

    discos[d].estoque = discos[d].estoque - diferenca;
    vendas[i].quantidade = novaQuantidade;
    vendas[i].valorTotal = precoUnitario * novaQuantidade;

    salvarVendasCSV();
    salvarDiscosCSV();
    printf("Venda editada! Novo total: R$ %.2f\n", vendas[i].valorTotal);
}


void removerVenda() {
    int i;
    int d;
    int id;
    int posicao;

    printf("Digite o ID da venda que deseja remover: ");
    scanf("%d", &id);
    getchar();

    posicao = buscarIndiceVenda(id);
    if (posicao == -1) {
        printf("Venda nao encontrada.\n");
        return;
    }

    d = buscarIndiceDisco(vendas[posicao].idDisco);
    if (d != -1) {
        discos[d].estoque = discos[d].estoque + vendas[posicao].quantidade;   
    }

    
    for (i = posicao; i < totalVendas - 1; i++) {
        vendas[i] = vendas[i + 1];
    }

    totalVendas--;
    salvarVendasCSV();
    salvarDiscosCSV();
    printf("Venda removida e estoque devolvido!\n");
}


void menuVendas() {
    int opcao;

    do {
        limparTela();
        printf("===== MENU VENDAS =====\n");
        printf("1 - Cadastrar venda\n");
        printf("2 - Listar vendas\n");
        printf("3 - Buscar venda por cliente\n");
        printf("4 - Editar venda\n");
        printf("5 - Remover venda\n");
        printf("0 - Voltar\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: cadastrarVenda(); pausar(); break;
            case 2: listarVendas();   pausar(); break;
            case 3: buscarVenda();    pausar(); break;
            case 4: editarVenda();    pausar(); break;
            case 5: removerVenda();   pausar(); break;
            case 0: break;
            default: printf("Opcao invalida!\n"); pausar();
        }

    } while (opcao != 0);
}
