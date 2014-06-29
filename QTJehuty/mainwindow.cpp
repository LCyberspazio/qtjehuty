/* QTJehuty, a simple text encryption program
 * by Giovanni Santostefano
 * http://twocentssecurity.wordpress.com
 *
 * Jehuty integrates the official C code
 * implementation of Anubis cipher
 */


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QClipboard>
#include <QMessageBox>
#include <time.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    srand(time(NULL));
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * Generates a random string of charc-number of characters
 * if block == 1 then the string will be enclosed by [ ]
 * if block == 2 then the string will begin with \n ;)
 */
QString* randomString(int charc, int block)
{
    int i=0;
    QString *str=new QString();

    if(charc<=0)return NULL;

    if(block==0)
    {
        for(i=0; i<charc; i++)
            str->append((char)((rand()%60)+64));
    }
    else if(block==1)
    {
        str->append('è');
        for(i=0; i<charc; i++)
            str->append((char)((rand()%60)+64));
        str->append(']');
    }
    else if(block==2)
    {
        str->append('\n');
        for(i=0; i<charc; i++)
            str->append((char)((rand()%60)+64));
    }

    return str;
}

void MainWindow::on_encryptbutton_clicked()
{
    struct NESSIEstruct sp;
    unsigned char key[16];
    //key=(unsigned char*)malloc(16);

    char plaintext[1024];

    /*blocks are 16byte long. But with 16 bytes
     *there's a problem in encryption so I have
     *to use 1024byte... can't say why*/
    unsigned char plainblock[1024];
    unsigned char cipherblock[1024];

    char *p;
    int  j=0, k=0;
    int c=0;

    //printf("Jehuty Short Message Encryption\nby Giovanni Santostefano\n\n");

    /* Enter the key of max 16 chars*/
    //printf("Insert passphrase(max 16 chars)> ");
    //GetText(key, KEYSIZE);

    /* print on screen to make entered passphrase unvisible */
    //for(i=0;i<6000; i++)printf("\n\n\n"/*,6000-i*/);

    memset(key,0,16);
    memcpy(key,ui->passphraseline->text().toStdString().c_str(),ui->passphraseline->text().length());

    /* Create Anubis key based on the passphrase */

    //struct NESSIEstruct* const spr=&sp;
    //::NESSIEkeysetup( (unsigned char* const)key, (struct NESSIEstruct* const)&sp);
    NESSIEkeysetup( key, &sp);



    /* Adding the salt to the string both beginning and ending */
    QString *rndtext=randomString((int)(rand()%5)+1,1);
    QString elabtext;
    elabtext=*rndtext+ui->cleartextline->text();
    delete rndtext;
    rndtext=randomString((int)(rand()%42)+15,2);
    elabtext+=*rndtext;
    delete rndtext;

    memset(plaintext,0,1024);
    strcpy(plaintext, /*ui->cleartextline->text()*/elabtext.toStdString().c_str());
    ui->ciphertextline->clear();

    p=plaintext;
    j=0;
    memset(plainblock,0,1024);
    memset(cipherblock,0,1024);

    while(*p!='\0')
    {
        plainblock[j++]=*p++;

        /* 128bit block filled, encrypt */
        if(j>=16 || *p=='\0')
        {
            j=0;
            NESSIEencrypt(&sp, plainblock, cipherblock);

            /* print the encrypted 128bit block in human readable chars */
            for(k=0; k<16; k++)
            {
                c=cipherblock[k];
                char g=(char)('!'+(c & 0x0f));
                ui->ciphertextline->setText(ui->ciphertextline->text().append(g));
                g=(char)('!'+((c & 0xf0)>>4));
                ui->ciphertextline->setText(ui->ciphertextline->text().append(g));
            }

            memset(plainblock,0,1024);
            memset(cipherblock,0,1024);
        }

    }

    if(ui->checkBox->checkState()==Qt::Checked)
    {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(ui->ciphertextline->text());
    }

    ui->conversation->appendPlainText("You: ");
    ui->conversation->appendPlainText(ui->cleartextline->text());
    ui->conversation->appendPlainText("---");

    return;
}

void MainWindow::on_decryptbutton_clicked()
{
    struct NESSIEstruct sp;
    unsigned char key[16];
    char plaintext[4096];
    unsigned char plainblock[16];
    unsigned char cipherblock[16];
    unsigned char plainwrite[18];
    char ciphertext[4096];
    char ciphertextb64[14096];
    char *p, *l;
    int i=0, j=0, k=0, iterations=0;
    char c=0;

    memset(key,0,16);
    memcpy(key,ui->passphraseline->text().toStdString().c_str(),ui->passphraseline->text().length());

    /* Create Anubis key based on the passphrase */
    NESSIEkeysetup(key, &sp);


    printf("\n\n\n\n\n\n\nJehuty Decrypt text\nby Giovanni Santostefano\n\n");

    memset(plaintext,0,4096);
    memset(ciphertextb64,0,14096);
    memset(ciphertext,0,4096);

    strcpy(ciphertextb64, ui->ciphertextline->text().toStdString().c_str());
    ui->cleartextline->clear();

    /* reconstructing the binary encrypted message
             * starting from the human readable version
             */
    p=ciphertextb64;
    l=ciphertext;
    iterations=0;
    while(*p!='\0')
    {
        c=(char)(((*(p+1)-'!')<<4)) + ((*p-'!'));
        p=p+2;
        *l=c;
        l++;
        iterations++;
    }

    /*printf("ITERATIONS: %d\n\n",iterations);*/

    /* Decrypting the message splitting it into 128bit blocks */
    p=ciphertext;
    j=0;
    memset(plainblock,0,16);
    memset(cipherblock,0,16);

    QString output;
    int flag=0; //flag if a \n is found, then exit from the cycle
    for(k=0; k<iterations; k++)
    {
        cipherblock[j]=*p++;

        /* 128bit block filled, decrypt it */
        if(j>=15)
        {
            j=0;
            NESSIEdecrypt(&sp, cipherblock, plainblock);
            memcpy(plainwrite, plainblock, 16);
            plainwrite[17]='\0';
            //printf("%s", plainwrite);
            for(i=0; i<17; i++)
            {
                if(plainwrite[i]=='\n' && flag==0)
                {
                    //memset(plainwrite+(i-1),0,18-i);
                    ///plainwrite[i-2]='\0';
                    plainwrite[i]='\0';
                    flag=1;
                }
                if(flag==1)
                    plainwrite[i]='\0';
            }

            output+=QString((const char*)plainwrite);
            //ui->cleartextline->setText(ui->cleartextline->text()+QString((const char*)plainwrite));



            memset(plainwrite,0,18);
            memset(plainblock,0,16);
            memset(cipherblock,0,16);

            //a \n was found... no more decoding
            if(flag==1)break;
        }
        else j++;
    }

    flag=0;
    QString output_def;
    for(i=0; i<output.length(); i++)
    {
        if(flag==0)
        {
            if(output.at(i)==']')
                flag=1;
        }
        else
        {
            output_def.append(output.at(i));
        }
    }


    ui->cleartextline->setText(output_def);

    ui->conversation->appendPlainText("Proxy: ");
    ui->conversation->appendPlainText(ui->cleartextline->text());
    ui->conversation->appendPlainText("---");

    return;
}

void MainWindow::on_pushButton_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("QTJehuty by Giovanni Santostefano\nvisit: http://twocentssecurity.wordpress.com/\n\nreleased under GPLv3");
    msgBox.exec();
}
