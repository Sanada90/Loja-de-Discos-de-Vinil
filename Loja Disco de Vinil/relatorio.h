
#include <stdio.h>

int matrizFaixa[50][4];                  

void montarMatriz() {
    int i;
    int j;
    int d;                               
    int c;                              
    int faixa;

    
    for (i = 0; i < 50; i++) {
        for (j = 0; j < 4; j++) {
            matrizFaixa[i][j] = 0;
        }
    }

    for (i = 0; i < totalVendas; i++) {
        d = buscarIndiceDisco(vendas[i].idDisco);
        c = buscarIndiceCliente(vendas[i].idCliente);

        if (d != -1 && c != -1) {
            if (clientes[c].idade <= 25) {
                faixa = 0;
            } else if (clientes[c].idade <= 35) {
                faixa = 1;
            } else if (clientes[c].idade <= 50) {
                faixa = 2;
            } else {
                faixa = 3;
            }

            matrizFaixa[d][faixa] = matrizFaixa[d][faixa] + vendas[i].quantidade;
        }
    }
}


void relatorioResumo() {
    int i;
    int discosVendidos = 0;
    float faturamento = 0;
    float valorEstoque = 0;

    for (i = 0; i < totalVendas; i++) {
        discosVendidos = discosVendidos + vendas[i].quantidade;
        faturamento = faturamento + vendas[i].valorTotal;
    }

    for (i = 0; i < totalDiscos; i++) {
        valorEstoque = valorEstoque + discos[i].preco * discos[i].estoque;
    }

    printf("\n--- RESUMO DA LOJA ---\n");
    printf("Vendas realizadas: %d\n", totalVendas);
    printf("Discos vendidos: %d\n", discosVendidos);
    printf("Faturamento total: R$ %.2f\n", faturamento);

    if (totalVendas > 0) {
        printf("Ticket medio por venda: R$ %.2f\n", faturamento / totalVendas);
    }

    printf("Valor parado em estoque: R$ %.2f\n", valorEstoque);
}


void relatorioMatriz() {
    int i;
    int total;

    montarMatriz();

    printf("\n--- DISCOS VENDIDOS POR FAIXA ETARIA ---\n");
    printf("%-28s %7s %7s %7s %7s %7s\n", "Disco", "ate 25", "26-35", "36-50", "51+", "Total");

    for (i = 0; i < totalDiscos; i++) {
        total = matrizFaixa[i][0] + matrizFaixa[i][1] + matrizFaixa[i][2] + matrizFaixa[i][3];

        printf("%-28.28s %7d %7d %7d %7d %7d\n", discos[i].titulo,
               matrizFaixa[i][0], matrizFaixa[i][1], matrizFaixa[i][2],
               matrizFaixa[i][3], total);
    }
}


void relatorioRanking() {
    int i;
    int j;
    int troca;
    int ordem[50];                      
    int totais[50];                      

    montarMatriz();

    for (i = 0; i < totalDiscos; i++) {
        ordem[i] = i;
        totais[i] = matrizFaixa[i][0] + matrizFaixa[i][1] + matrizFaixa[i][2] + matrizFaixa[i][3];
    }

   
    for (i = 0; i < totalDiscos - 1; i++) {
        for (j = 0; j < totalDiscos - 1 - i; j++) {
            if (totais[j] < totais[j + 1]) {
                troca = totais[j];
                totais[j] = totais[j + 1];
                totais[j + 1] = troca;

                troca = ordem[j];
                ordem[j] = ordem[j + 1];
                ordem[j + 1] = troca;
            }
        }
    }

    printf("\n--- RANKING DE DISCOS MAIS VENDIDOS ---\n");

    for (i = 0; i < totalDiscos; i++) {
        printf("%2d. %-28.28s %-20.20s %3d vendido(s)\n", i + 1, discos[ordem[i]].titulo,
               discos[ordem[i]].artista, totais[i]);
    }
}


void menuRelatorios() {
    int opcao;

    do {
        limparTela();
        printf("===== MENU RELATORIOS =====\n");
        printf("1 - Resumo da loja\n");
        printf("2 - Discos por faixa etaria (matriz)\n");
        printf("3 - Ranking de discos mais vendidos\n");
        printf("0 - Voltar\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: relatorioResumo();  pausar(); break;
            case 2: relatorioMatriz();  pausar(); break;
            case 3: relatorioRanking(); pausar(); break;
            case 0: break;
            default: printf("Opcao invalida!\n"); pausar();
        }

    } while (opcao != 0);
}
