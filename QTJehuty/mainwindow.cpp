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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
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



    memset(plaintext,0,1024);
    strcpy(plaintext, ui->cleartextline->text().toStdString().c_str());
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
    int j=0, k=0, iterations=0;
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
            ui->cleartextline->setText(ui->cleartextline->text()+QString((const char*)plainwrite));


            memset(plainwrite,0,18);
            memset(plainblock,0,16);
            memset(cipherblock,0,16);
        }
        else j++;
    }


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
