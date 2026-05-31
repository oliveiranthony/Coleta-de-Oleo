#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct {
    char name[100], cpf[15], phone[20];
} Cadaster;

typedef struct {
    unsigned int quantity;
    float liters;
} Register;

void DateTimeCadaster(char *buffer, size_t sizeMax) {
    time_t moment;
    struct tm *infoTime;
    time(&moment);
    infoTime = localtime(&moment);
    strftime(buffer, sizeMax, "%d/%m/%Y %H:%M:%S", infoTime);
}

void ClearBuffer() {
    int trash;
    while ((trash = getchar()) != '\n' && trash != EOF)
        ;
}

/*void TotalRaised(int assistant) {
    assistant = 0;
    if()
}*/

int main() {

    FILE *arquivo;
    arquivo = fopen("CadastroDoador.txt", "a");
    Cadaster cad;
    Register reg;
    int opcao = 0;

    while (opcao < 1 || opcao > 5) {
        printf("===== SISTEMA DE DOACAO DE OLEO =====\n");
        printf("1 - Cadastrar doador.\n");
        printf("2 - Registrar doacao.\n");
        printf("3 - Relatorios.\n");
        printf("4 - Total arrecadado.\n");
        printf("5 - Sair... .\n");
        printf("Digite a opcao desejada: ");

        scanf("%d", &opcao);
        ClearBuffer();
    }
    fclose(arquivo);

    switch (opcao) {
    case 1:
        arquivo = fopen("CadastroDoador.txt", "a+");
        if (arquivo == NULL)
        {
            printf("Erro ao abrir o arquivo.\n");
            return 1;
        }
        printf("\nCadastro de doador:\n");

        printf("Insira seu nome completo: ");
        fgets(cad.name, 100, stdin);
        cad.name[strcspn(cad.name, "\n")] = '\0';

        printf("Informe seu cpf: ");
        fgets(cad.cpf, 15, stdin);
        cad.cpf[strcspn(cad.cpf, "\n")] = '\0';

        printf("Telefone de registro: ");
        fgets(cad.phone, 20, stdin);
        cad.phone[strcspn(cad.phone, "\n")] = '\0';

        fprintf(arquivo, "Nome: %s\n"
                         "CPF: %s\n"
                         "Telefone: %s\n",
                cad.name, cad.cpf, cad.phone);
        char data_atual[20];
        DateTimeCadaster(data_atual, sizeof(data_atual));
        fprintf(arquivo, "Momento do cadastro: %s\n\n", data_atual);
        fclose(arquivo);
        printf("Cadastro realizado com sucesso!\n");
        break;

    case 2:
        arquivo = fopen("RegistroDoacao.txt", "a+");
        printf("\nRegistro de doacao!\n");
        printf("Quantidade de garrafas: ");
        scanf("%u", &reg.quantity);

        printf("Digite a quantidade de litros: ");
        scanf("%f", &reg.liters);

        fclose(arquivo);

        break;

    case 3:
        arquivo = fopen("relatorio.txt", "r+");
        printf("\nRelatorios!\n");
        break;

    case 4:
        arquivo = fopen("registro.txt", "r+");
        printf("\nTotal arrecadado:\n");
        break;

    case 5:
        printf("\nSaindo do sistema...\n");
        return 1;
    }
    return 0;
}
