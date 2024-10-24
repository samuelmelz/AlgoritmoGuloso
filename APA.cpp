#include <iostream>
//#include <conio.h>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <iterator>
#include <string.h>
#include <bits/stdc++.h>
#define maximo 10000

using namespace std;
char Linha[10000];

typedef struct processo processo;
typedef struct servidor servidor;

struct processo{
	char Nome[5];
	vector<int> t;
 	vector<int> c;
 	bool alocado;
}; 

struct servidor{
	char Nome[5];
	int capacidade;
 	vector<processo*> processos;
}; 

processo *criarProcesso(int pos)
{
	processo *Proc = (processo*)malloc(sizeof(processo));
	sprintf(Proc->Nome, "P%d", pos);
	Proc->alocado=false;
	Proc->c = {};
	Proc->t = {};
	return Proc;
}

servidor *criarServidor(int pos, int capacidade)
{
	servidor *Serv = (servidor*)malloc(sizeof(servidor));
	sprintf(Serv->Nome, "S%d", pos);
	Serv->capacidade = capacidade;
	Serv->processos = {};
	return Serv;
}

int menorValor(vector<int> vetor, int posicao)
{
	sort(vetor.begin(), vetor.end());
	//Retorna o primeiro ou segundo ou terceiro..... menor elemento
	return vetor[posicao];
}

int posicaoMenorValor(vector<int> vetor, int valor)
{
	int posicao;
	for(int i = 0; i<vetor.size(); i++)
	{
		if(vetor[i]==valor)
		{
			posicao = i;
		}
	}
	return posicao;
}

int valorDeCustoAlgoritmoguloso(int nJobs, int nServidores, vector<servidor *> servidores, vector<processo *> processos, int custoP){
    int custo = 0;
	
	//Percorre os processos buscando em qual servidor o processo terá menor custo
	for(int i = 0; i < nJobs; i++)
	{
		int servidorEscolhido;
		int j = 0;
		while(true)
		{
		    //Retorna o j-ésimo maior valor do vector c
			int maior = menorValor(processos[i]->c, j);
			servidorEscolhido = posicaoMenorValor(processos[i]-> c, maior);
			
			//Se houver capacidade
			if(servidores[servidorEscolhido]->capacidade >= processos[i]->t[servidorEscolhido])
			{
				servidores[servidorEscolhido]->processos.push_back(processos[i]);
				servidores[servidorEscolhido]->capacidade -= processos[i]->t[servidorEscolhido];
				processos[i]->alocado = true;
				custo += processos[i]->c[j];
				break;
			}
			else
			{
				j++;
				
				//Limitador de tentativas para evitar loop infinito
				if(j > nServidores)
				{
					custo += custoP;
					break;
				}
			}
		}
	}
    return custo;
}

void algoritmoGuloso(int nJobs, int nServidores, vector<servidor*> servidores, vector<processo*> processos, int custoP)
{
    //	Ideia: Percorrer o vector de custo de cada processo e ordenar pelo tempo. 
    //	Em seguida verificar se aquele servidor tem capacidade para receber aquele processo. Se sim, recebe, senão, pega o segundo, terceiro... com menor custo e assim vai.
	int custo = 0;
	
	//Percorre os processos buscando em qual servidor o processo terá menor custo
	for(int i = 0; i < nJobs; i++)
	{
		int servidorEscolhido;
		int j = 0;
		while(true)
		{
		    //Retorna o j-ésimo maior valor do vector c
			int maior = menorValor(processos[i]->c, j);
			servidorEscolhido = posicaoMenorValor(processos[i]-> c, maior);
			
			//Se houver capacidade
			if(servidores[servidorEscolhido]->capacidade >= processos[i]->t[servidorEscolhido])
			{
				servidores[servidorEscolhido]->processos.push_back(processos[i]);
				servidores[servidorEscolhido]->capacidade -= processos[i]->t[servidorEscolhido];
				processos[i]->alocado = true;
				custo += processos[i]->c[j];
				break;
			}
			else
			{
				j++;
				
				//Limitador de tentativas para evitar loop infinito
				if(j > nServidores)
				{
					custo += custoP;
					break;
				}
			}
		}
	}
	
	cout << "Custo da solucao:\t" << custo << endl; 
	for(int i = 0; i < nServidores; i++)
	{
		cout << "Servidor " << servidores[i]->Nome << endl;
		for(int j = 0; j < servidores[i]->processos.size(); j++)
		{
			cout << servidores[i]->processos[j]->Nome << endl;	
		}
		cout << "Capacidade Restante:\t" << servidores[i]->capacidade << endl << endl;
	}
	
	cout << "Processos sem alocacao no servidor:\n";
	for(int i =0; i<processos.size(); i++)
	{
		if(processos[i]->alocado == false)
		{
			cout << processos[i]->Nome << endl;
		}
	}
}

//funcao vnd
int vnd(int nJobs, int nServidores, vector<servidor *> servidores, vector<processo *> processos, int custoP)
{
    int valorAtual = valorDeCustoAlgoritmoguloso(nJobs,nServidores, servidores, processos,custoP);

        vector<processo* > processoAux;
		while(true){
			int j = rand() % processos.size();
			int i = rand() % processos.size();

            processoAux = processos;
			//faz o swap
			processo* temp = processoAux[i];
			processoAux[i] = processoAux[j];
			processoAux[j] = temp;
				
			int novo = valorDeCustoAlgoritmoguloso(nJobs,nServidores, servidores, processoAux,custoP);

			if (novo < valorAtual){
				valorAtual = novo;
				processos = processoAux;
				continue;
			}else{
				int novoJ=0;
				novo = valorDeCustoAlgoritmoguloso(nJobs,nServidores, servidores, processoAux,custoP);

                //Espelhamento
				for(int i = processos.size()-1; i>novoJ; i--, novoJ++){
					processo* temp = processoAux[i];
					processoAux[i] = processoAux[novoJ];
					processoAux[novoJ] = temp;
				}	
				novo = valorDeCustoAlgoritmoguloso(nJobs,nServidores, servidores, processoAux,custoP);
				if (novo < valorAtual){
    				valorAtual = novo;
    				processos = processoAux;
    				continue;
    			}else{
    				break;
    			}
		}
	}
	    algoritmoGuloso(nJobs,nServidores, servidores, processos,custoP);
		return valorAtual;
}	

int main()
{
	servidor* server;
	processo* proc;
	vector<servidor*> servidores;
	vector<processo*> processos;	
    FILE *arq;
  // Abre um arquivo TEXTO para LEITURA
  arq = fopen("ArqTeste.txt", "rt");
  if (arq == NULL)  // Se houve erro na abertura
  {
		printf("Problemas na abertura do arquivo\n");
	   	return -1;
  }
  
  //nJobs
  int n = atoi(fgets(Linha, maximo, arq));
  //nServidor
  int m = atoi(fgets(Linha, maximo, arq));
  //custo de nao alocacao
  int p = atoi(fgets(Linha, maximo, arq));
  fgets(Linha, maximo, arq);
  
  /* b */
  fgets(Linha, maximo, arq);
  char separador[2] = " ";
  char *split;
  //Separa a Linha em substrings separadas pelo separador, no caso, espaço
  split = strtok(Linha, separador);
  while( split != NULL ) {
  	 server = criarServidor(servidores.size(), atoi(split));
  	 servidores.push_back(server);
     split = strtok(NULL, separador);
  }
  /* b */
  fgets(Linha, maximo, arq);
  
  //Cria processos e adiciona ao vector processos
  for(int i =0; i < n; i++)
  {
  	proc = criarProcesso(i);
  	processos.push_back(proc);
  }
  
  /* t */
  //Lê a linha
  fgets(Linha, maximo, arq);
  //Executa para cada linha da matriz t
  for(int i = 0; i < m; i++)
  {
  	int j=0;
  	split = strtok(Linha, separador);
  	//Enquanto houver dados na linha
	while(split!=NULL) {
		//Insere no vector t o valor de t na posição ij
		processos[j]->t.push_back(atoi(split));
	    split = strtok(NULL, separador);
	    j++;
	}
	//Avança para a proxima linha
  	fgets(Linha, maximo, arq);
  }
  /* t */
  
  /* c */
  //Lê a linha
  fgets(Linha, maximo, arq);
  //Executa para cada linha da matriz c
  for(int i = 0; i < m; i++)
  {
  	int j=0;
  	split = strtok(Linha, separador);
  	//Enquanto houver dados na linha
	while(split!=NULL) {
		//Insere no vector c o valor de c na posição ij
		processos[j]->c.push_back(atoi(split));
	    split = strtok(NULL, separador);
	    j++;
	}
	//Avança para a proxima linha
  	fgets(Linha, maximo, arq);
  }
  /* c */
  std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
  algoritmoGuloso(n, m, servidores, processos, p);
  std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

    std::chrono::duration<double> dt = 
            std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1);
        std::cout  << "\n\nTempo de execucao: " <<  dt.count() * 1000 << " ms" << std::endl;
  
  fclose(arq);
}
