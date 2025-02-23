#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
using namespace std;

int main()
{
    string nome;

    cout << "JarCreator" << endl;
    cout << "Digite o nome do arquivo (ex: app/Main se estiver em uma pasta): ";
    getline(cin, nome);

    ifstream arquivo(nome + ".java");

    if (arquivo.is_open())
    {
        size_t pos = nome.find('/');
        if (pos == string::npos) pos = nome.find('\\');

        string pacote, classe;
        if (pos != string::npos) {
            pacote = nome.substr(0, pos);
            classe = nome.substr(pos + 1);
        } else {
            classe = nome;
        }

        ofstream manifest((pacote.empty() ? "" : pacote + "/") + "MANIFEST.MF");
        if (manifest.is_open())
        {
            if (!pacote.empty()) {
                manifest << "Main-Class: " << pacote << "." << classe << endl;
            } else {
                manifest << "Main-Class: " << classe << endl;
            }
            manifest.close();
        }
        else
        {
            cout << "Erro ao criar o MANIFEST.MF!" << endl;
            return 1;
        }

        string separator = "/";
    #ifdef _WIN32
        separator = "\\";
    #endif

        string init1 = "javac " + nome + ".java";
        system(init1.c_str());

        string init2 = "jar cfm " + (pacote.empty() ? "" : pacote + "/") + classe + ".jar " + (pacote.empty() ? "" : pacote + "/") + "MANIFEST.MF " + (pacote.empty() ? "" : pacote + "/") + classe + ".class";
        system(init2.c_str());

        string init3 = "jar tf " + (pacote.empty() ? "" : pacote + "/") + classe + ".jar";
        system(init3.c_str());

        string init4 = "java -jar " + (pacote.empty() ? "" : pacote + "/") + classe + ".jar";
        system(init4.c_str());
    }
    else
    {
        cout << "Erro! Arquivo " << nome << ".java não encontrado!" << endl;
    }

    return 0;
}
