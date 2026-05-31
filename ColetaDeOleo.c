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
    while ((trash = getchar()) != '\n' && trash != EOF);
}

int searchDonor(Cadaster *cad, char search[]) {

    FILE *cadasterFile = fopen("CadastroDoador.txt", "r");

    if (cadasterFile == NULL) {
        return 0;
    }

    char line[200];

    while (fgets(line, sizeof(line), cadasterFile)) {

        if (strncmp(line, "Nome: ", 6) == 0) {
            strcpy(cad->name, line + 6);
            cad->name[strcspn(cad->name, "\n")] = '\0';
        }

        else if (strncmp(line, "CPF: ", 5) == 0) {
            strcpy(cad->cpf, line + 5);
            cad->cpf[strcspn(cad->cpf, "\n")] = '\0';

            if (strcmp(search, cad->name) == 0 ||
                strcmp(search, cad->cpf) == 0) {

                fclose(cadasterFile);
                return 1;
            }
        }
    }

    fclose(cadasterFile);
    return 0;
}

int main() {

    FILE *cadasterFile, *registerFile, *reportFile, *totalFile;
    cadasterFile = fopen("CadastroDoador.txt", "a");
    registerFile = fopen("RegistroDoacao.txt", "a");
    Cadaster cad;
    Register reg;
    int option = 0;

    while (option < 1 || option > 5) {
        printf("===== SISTEMA DE DOACAO DE OLEO =====\n");
        printf("1 - Cadastrar doador.\n");
        printf("2 - Registrar doacao.\n");
        printf("3 - Relatorios.\n");
        printf("4 - Total arrecadado.\n");
        printf("5 - Sair... .\n");
        printf("Digite a opcao desejada: ");

        scanf("%d", &option);
        ClearBuffer();
    }
    fclose(cadasterFile);

    switch (option) {
    case 1:
        cadasterFile = fopen("CadastroDoador.txt", "a+");
        if (cadasterFile == NULL){
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

        fprintf(cadasterFile, "Nome: %s\n"
                         "CPF: %s\n"
                         "Telefone: %s\n",
                cad.name, cad.cpf, cad.phone);
        char currentDate[20];
        DateTimeCadaster(currentDate, sizeof(currentDate));
        fprintf(cadasterFile, "Momento do cadastro: %s\n\n", currentDate);
        fclose(cadasterFile);
        printf("Cadastro realizado com sucesso!\n");
        break;

    case 2:
        char search[100]; float totalPerUser = 0.0;
        registerFile = fopen("RegistroDoacao.txt", "a+");
        printf("\nRegistro de doacao:\n");
        printf("Buscar doador (Nome ou CPF): ");
        fgets(search, 100, stdin);
        search[strcspn(search, "\n")] = '\0';
        if(!searchDonor(&cad, search)) {
            printf("Doador nao encontrado. Por favor, cadastre-se primeiro.\n");
            break;
        }
        printf("Doador encontrado!\n");

        printf("Quantidade de garrafas doadas: ");
        scanf("%u", &reg.quantity);
        ClearBuffer();
        printf("Informe a capacidade da garrafa (em litros): ");
        scanf("%f", &reg.liters);
        ClearBuffer();
        totalPerUser = reg.liters * reg.quantity;
        printf("Total doado neste registro: %.2f litros\n", totalPerUser);
        fprintf(registerFile, "Doador: %s\n"
                         "Quantidade de garrafas: %u\n"
                         "Capacidade da(s) garrafa(s) doada(s): %.2f litros\n"
                         "Total doado neste registro: %.2f litros\n",
                        cad.name, reg.quantity, reg.liters, totalPerUser);
        
        DateTimeCadaster(currentDate, sizeof(currentDate));
        fprintf(registerFile, "Doação realizada em: %s\n\n", currentDate);
        fclose(registerFile);
        break;

    case 3:
        reportFile = fopen("relatorio.txt", "r");
        reportFile = registerFile;
        printf("\nRelatorios!\n");
        break;

    case 4:
        totalFile = fopen("registro.txt", "r+");
        printf("\nTotal arrecadado:\n");
        break;

    case 5:
        printf("\nSaindo do sistema...\n");
        return 1;
    }

    return 0;
}