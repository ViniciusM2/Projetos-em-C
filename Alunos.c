#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h> /* Função exit*/
#include <ctype.h> /* Função toupper*/
#include <locale.h>

#define ARQ "Dados.Dat" /* Arquivo com os dados*/

#define OP_INSERIR '1'
#define OP_ALTERAR '2'
#define OP_APAGAR  '3'
#define OP_LISTAR '4'
#define OP_PESQUISAR '5'
#define OP_LISTAR_REPROVADOS '6'
#define OP_LISTAR_APROVADOS '7'


#define OP_SAIR '0'

#define OP_PESQ_CURSO '1'
#define OP_PESQ_NOME '2'




char *MainMenu[]={
	"1. Inserir Aluno",
	"2. Alterar Aluno",
	"3. Apagar Aluno",
	"4. Listar Alunos",
	"5. Pesquisas",
	"6. Listar Alunos Reprovados",
	"7. Listar Alunos Aprovados",
	"0. Sair",
	NULL	/* Acabaram as opções*/
};

char *PesqMenu[]={
	"1. Pesquisar por Curso",
	"2. Pesquisar por Nome",
	"0. Voltar",
	NULL	/* Acabaram as opções*/ 	
};

FILE *fp; 

typedef struct
{
	int n_registro;
	char Nome[30+1];
	char Curso[30+1];
	char Cadeira[30+1];
	char Matricula[30+1];
	int Faltas;
	int P;
	float Nota1;
	float Nota2;
	float Media=0;
	char Status; 	/*		'*' indica que o registro será apagado		*/
}ALUNO;

void Mensagem(char *msg);

void Ler_Aluno(ALUNO *a)
{
	printf("Nome do Aluno	: ");gets(a->Nome);
	printf("Curso	: ");gets(a->Curso);
	printf("Nome da Disciplina	: ");gets(a->Cadeira);
	printf("Matrícula	: ");gets(a->Matricula);
	printf("Número de Faltas	: ");scanf("%d",&a->Faltas);fflush(stdin);
	printf("Período	: ");scanf("%d",&a->P);fflush(stdin);
	printf("Primeira Nota	: ");scanf("%f",&a->Nota1);fflush(stdin);
	printf("Segunda Nota	: ");scanf("%f",&a->Nota2);fflush(stdin);	
	a->Media=((a->Nota1)+(a->Nota2))/2;		
	a->Status=' ';
	fflush(stdin);
}

void Mostrar_Aluno(ALUNO a)
{
	puts("\n\n");
	printf("Número de Registro:%d  \n\n",a.n_registro+1);
	printf("Nome do Aluno   :%s \n\n",a.Nome);
	printf("Curso	:%s \n\n",a.Curso);
	printf("Nome da Disciplina	:%s  \n\n",a.Cadeira);
	printf("Matrícula	:%s \n\n",a.Matricula);
	printf("Número de Faltas:%d \n\n",a.Faltas);
	printf("Período	   : %d \n\n",a.P);
	printf("Primeira Nota	: %f  \n\n",a.Nota1);
	printf("Segunda Nota    : %f  \n\n",a.Nota2);
	printf("Media	: %f  \n\n",a.Media);
	
if(a.Media>=7)
	printf("ALUNO APROVADO\n\n");
else
	printf("ALUNO REPROVADO\n\n");
}

void Adicionar_Aluno (ALUNO a)
{
	fseek(fp,0L, SEEK_END);
	if(fwrite(&a,sizeof(a),1,fp)!=1)
		Mensagem("adicionar Aluno: Falhou a escrita do registro");
}

void Mensagem(char *msg)
{
	printf(msg);
	getchar();
}

void Inic()
{
	fp=fopen(ARQ,"r+b"); /*Tentar Abrir*/
	if(fp==NULL)
	{
		fp=fopen(ARQ,"w+b");
		if(fp==NULL)
		{
			fprintf(stderr,"ERRO FATAL: Impossível Criar Arquivo de Dados\n");
			exit(1);
		}
	}
}

char Menu(char *Opcoes[])
 {
 	int i;
 	char ch;
 	
 	while(1)
 	{  /*Cls*/
 		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
 		for(i=0;Opcoes[i]!=NULL; i++)
 		printf("\t\t%s\n\n",Opcoes[i]);
 		
 	printf("\n\n\tOpção: ");
 	ch=getchar(); fflush(stdin);
 	for(i=0; Opcoes[i]!=NULL; i++)
 	if(Opcoes[i][0]==ch)
 		return ch;
	 }
 }
 
 void Inserir_Aluno()
 {
 	ALUNO x;
 	Ler_Aluno(&x);
 	Adicionar_Aluno(x);
 }
 
 void Alterar_Aluno()
 {
 	ALUNO x;
 	long int n_reg;
	printf("Qual o Nº do Registro: ");
	scanf("%ld", &n_reg);fflush(stdin);
	if(fseek(fp, (n_reg-1)*sizeof(ALUNO),SEEK_SET)!=0)
		{
		Mensagem("Registro Inexistente ou Problemas no Posicionamento!!!");
		return;	
			
		}
	if(fread(&x,sizeof(ALUNO),1,fp)!=1)
	{
		Mensagem("Problemas na leitura do Registro!!!");
		return;
	}
	if(x.Status=='*')
	{
		Mensagem("Um Registro Apagado não pode ser alterado!!!\n\n");
		return;
	}
		
	printf("\n\nDados Atuais\n\n");
	Mostrar_Aluno(x);
	printf("\n\nNovos Dados\n\n");
	Ler_Aluno(&x);
	
	//Recuar um Registro no Arquivo
	fseek(fp,-(long) sizeof(ALUNO),SEEK_CUR);
	//Reescrever o Registro;
	fwrite(&x,sizeof(ALUNO),1,fp);
	fflush(fp); /*Despejar os Dados no Disco Rígido*/	 
 }
 
 
 void Apagar_Aluno()
 {
 	ALUNO x;
 	long int n_reg;
 	char resp;
 	
 	printf("Qual o Nº do Registro:");
 	scanf("%ld", &n_reg);fflush(stdin);
 	
 	if(fseek(fp, (n_reg-1)*sizeof(ALUNO),SEEK_SET)!=0)
		{
		Mensagem("Registro Inexistente ou Problemas no Posicionamento!!!");
		return;	
			
		}
	if(fread(&x,sizeof(ALUNO),1,fp)!=1)
	{
		Mensagem("Problemas na leitura do Registro!!!");
		return;
	}
	if(x.Status=='*')
	{
		Mensagem("O registro já está apagado!!!\n\n");
		return;
	}
	
	printf("\n\nDados Atuais\n\n");
	Mostrar_Aluno(x);
	
	printf("\n\nApagar o Registro (s/n)???: "); resp=getchar(); fflush(stdin);
	if (toupper(resp)!='S') return;
	
	x.Status='*';
		//Recuar um Registro no Arquivo
	fseek(fp,-(long) sizeof(ALUNO),SEEK_CUR);
	//Reescrever o Registro;
	fwrite(&x,sizeof(ALUNO),1,fp);
	fflush(fp); /*Despejar os Dados no Disco Rígido*/	
	
 }
 
 
 void Listar()
 {
 	long int N_Linhas=0;
 	ALUNO reg;
 	rewind(fp);
 	while(1)
 	{
 		if (fread(&reg,sizeof(reg),1,fp)!=1)break; /*Sair do Laço*/
 		if (reg.Status=='*')continue; /*Passa ao próximo registro*/
 		Mostrar_Aluno(reg);
 		N_Linhas++;
 		if(N_Linhas%20==0)
 		Mensagem("PESSIONE <ENTER> para continuar	.	.	. ");
	 }
	Mensagem("PESSIONE <ENTER> para continuar	.	.	. "); /*No fim da Listagem*/ 
 }
 
 void Pesquisar_Nome(char *s)
 {
 	ALUNO reg;
 	rewind(fp);
 	
 	while(fread(&reg,sizeof(ALUNO),1,fp))
 	if(reg.Status!='*'&&strstr(reg.Nome,s))
 		Mostrar_Aluno(reg);
 		
 	Mensagem("PESSIONE <ENTER> para continuar	.	.	. "); /*No fim da Listagem*/	
 }
 
 void Pesquisar_Curso(char *s)
 {
 	ALUNO reg;
 	rewind(fp);
 	
 	while(fread(&reg,sizeof(ALUNO),1,fp))
 	if(reg.Status!='*'&&strstr(reg.Curso,s))
 		Mostrar_Aluno(reg);
 		
 	Mensagem("PESSIONE <ENTER> para continuar	.	.	. "); /*No fim da Listagem*/	
 }
 
 
 int dictstrcmp(char *str1, char *str2)
{
while(1)
	{
	char c1 = *str1++;
	char c2 = *str2++;

	if(isupper(c1))
		c1 = tolower(c1);

	if(isupper(c2))
		c2 = tolower(c2);

	if(c1 != c2)
		return c1 - c2;
	if(c1 == '\0')
		return 0;
	}
}


 void Copiar_Aluno(ALUNO *dest, ALUNO orig)
{
	int i=0;

	
		dest->Faltas=orig.Faltas;
		dest->P=orig.Faltas;
		dest->Nota1=orig.Nota1;
		dest->Nota2=orig.Nota2;
		dest->Media=orig.Media;
		dest->Status=orig.Status;
		
		strcpy(dest->Nome,orig.Nome);	
		strcpy(dest->Curso,orig.Curso);	
		strcpy(dest->Cadeira,orig.Cadeira);	
		strcpy(dest->Matricula,orig.Matricula);	
		strcpy(dest->Matricula,orig.Matricula);	
			
}
 
 void alunosort(ALUNO *alunos, int nstrings)
{
int i, j;
int didswap;
ALUNO tmp;

do	{
	didswap = 0;
	for(i = 0; i < nstrings - 1; i++)
		{
		j = i + 1;
		if(dictstrcmp(alunos[i].Nome, alunos[j].Nome) > 0)
			{
			Copiar_Aluno(&tmp,alunos[i]);
			Copiar_Aluno(&alunos[i],alunos[j]);
			Copiar_Aluno(&alunos[j],tmp);
			didswap=1;
			}
			
		}
	} while(didswap);
}










 
 
 void Reescrever()
{
	int i=0,j=0;
	ALUNO aluno_aux;
	ALUNO v[10];
	ALUNO w[10];
	rewind(fp);
	while(1)
	{
		if (fread(&aluno_aux,sizeof(ALUNO),1,fp)!=1)
		{
	//	printf("Término do processo de transferência do arquivo para RAM na %d-ésima tentativa",i);	
		break;
		}
		if(aluno_aux.Status!='*')
		{
			//Recuar um Registro no Arquivo
			fseek(fp,-(long) sizeof(ALUNO),SEEK_CUR);
	
			if(fread(&v[j],sizeof(ALUNO),1,fp)!=1)
			{
			//	printf("Erro no processo de reescrever na %d-ésima tentativa",i);
			}
			//printf("%d-ésimo elemento do vetor RAM adicionado com sucesso\n\n",j);
			j++;
		}
		i++;
	}
	
	/*
	for(i=0;i<j;i++)
	Mostrar_Aluno(v[i]);
		
	printf("\n\n\nTÉRMINO DA MOSTRA DE ALUNOS\n\n\n");
	*/
		alunosort(v,j);
		
		
	for(i=0;i<j;i++)
	{
	v[i].n_registro=i;
//	Mostrar_Aluno(v[i]);	
	}
	
	
	fclose(fp);
	
	fp=fopen(ARQ,"w+b");
		if(fp==NULL)
		{
			fprintf(stderr,"ERRO FATAL: Impossível Criar Arquivo de Dados\n");
			exit(1);
		}
		
	for(i=0;i<j;i++)
	{
		fwrite(&v[i],sizeof(ALUNO),1,fp);
	}
	
	fflush(fp);	
}

void Listar_Reprovados()
{
	int i=0,j=0;
	ALUNO aluno_aux;
	ALUNO v[10];
	ALUNO w[10];
	rewind(fp);
	while(1)
	{
		if (fread(&aluno_aux,sizeof(ALUNO),1,fp)!=1)
		{
		printf("Término do processo de transferência do arquivo para RAM na %d-ésima tentativa",i);	
		break;
		}
		if(aluno_aux.Status!='*'&&!(aluno_aux.Media>=7))
		{
			//Recuar um Registro no Arquivo
			fseek(fp,-(long) sizeof(ALUNO),SEEK_CUR);
	
			if(fread(&v[j],sizeof(ALUNO),1,fp)!=1)
			{
				printf("Erro no processo de reescrever na %d-ésima tentativa",i);
			}
			printf("%d-ésimo elemento do vetor RAM adicionado com sucesso\n\n",j);
			j++;
		}
		i++;
		
		
	}
	
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	for(i=0;i<j;i++)
	Mostrar_Aluno(v[i]);
	getchar();
}
void Listar_Aprovados()
{
	int i=0,j=0;
	ALUNO aluno_aux;
	ALUNO v[10];
	ALUNO w[10];
	rewind(fp);
	while(1)
	{
		if (fread(&aluno_aux,sizeof(ALUNO),1,fp)!=1)
		{
		printf("Término do processo de transferência do arquivo para RAM na %d-ésima tentativa",i);	
		break;
		}
		if(aluno_aux.Status!='*'&&(aluno_aux.Media>=7))
		{
			//Recuar um Registro no Arquivo
			fseek(fp,-(long) sizeof(ALUNO),SEEK_CUR);
	
			if(fread(&v[j],sizeof(ALUNO),1,fp)!=1)
			{
				printf("Erro no processo de reescrever na %d-ésima tentativa",i);
			}
			printf("%d-ésimo elemento do vetor RAM adicionado com sucesso\n\n",j);
			j++;
		}
		i++;
		
		
	}
	
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	for(i=0;i<j;i++)
	Mostrar_Aluno(v[i]);
	getchar();
}
  int main()
 {
 	setlocale(LC_ALL, "Portuguese");
 	char Opcao;
 	Inic();
 	Reescrever();
 	while((Opcao=Menu(MainMenu))!=OP_SAIR)
 		switch(Opcao)
 		{
 			case OP_INSERIR: Inserir_Aluno();break;
 			case OP_ALTERAR: Alterar_Aluno();break;
 			case OP_APAGAR:  Apagar_Aluno(); break;
 			case OP_LISTAR:  Listar(); break;
 			case OP_LISTAR_REPROVADOS: Listar_Reprovados(); break;
			case OP_LISTAR_APROVADOS: Listar_Aprovados(); break;
 			case OP_PESQUISAR:
 				while((Opcao=Menu(PesqMenu))!=OP_SAIR)
 				switch(Opcao)
 				{
 					case OP_PESQ_CURSO:
 						{
 							char string[BUFSIZ+1];
							printf("Qual o Curso a Procurar: ");
							gets(string);fflush(stdin);
							Pesquisar_Curso(string);break;
					 	}
					case OP_PESQ_NOME:
						{
							char string[BUFSIZ+1];
							printf("Qual o Nome a Procurar: ");
							gets(string);fflush(stdin);
							Pesquisar_Nome(string);break;
						}
				 }
		 }
 }

//Referências:

/*

"Gest.c", código resposta da questão 1 do capítulo 11 do livro "Linguagem C"(pág. 387-pág. 391) do autor Luis Damas, por Luis Damas. 
Foi utilizado como base da manipulação de arquivos e structs durante o código.

"http://www.eskimo.com/~scs/cclass/int/sx10b.html", "C programming class notes - 24.2 What are Function Pointers Good For?". 
Foi utilizado como base para o algoritmo da função de ordenção dos registros.

*/
