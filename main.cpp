#include "mainwindow.h"
#include <QApplication>
#include <string>
#include <iostream>
#include "utilityalgorithms.h"

/*
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
*/

using namespace std;

int main(int argc, char* argv[]){

    string s1 = "Kittens";
    string s2 = "Mittens";
    string s3 = "Mitts";

    int test = UtilityAlgorithms::longestCommonSS(s1, s2);

    cout << "LCSS -- " << s1 << ", " << s2 << " " << test << endl;

    test = UtilityAlgorithms::levDist(s1, s2);

    cout << "Levenshtein -- " << s1 << ", " << s2 << " " << test << endl;

    test = UtilityAlgorithms::longestCommonSS(s2, s3);

    cout << "LCSS -- " << s2 << ", " << s3 << " " << test << endl;

    test = UtilityAlgorithms::levDist(s2, s3);

    cout << "Levenshtein -- " << s2 << ", " << s3 << " " << test << endl;



    /* TOKENIZER
    if(argc < 2){
        //qDebug() << "File name required";
        cout << "File name required";
        return 1;
    }

    Tokenizer* tok = new Tokenizer(argv[1]);
    tok->tokenize();
    tok->writeToSQL();

    delete tok;
    */
}
