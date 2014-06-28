qtjehuty
========

QTJehuty an easy to use, QT4 base, simple conversation encryption program

QTJehuty is developed by
Giovanni Santostefano that integrates the 
Anubis cipher to encrypt/decrypt simple text.

 The Anubis algorithm was developed by
 pbarreto@scopus.com.br             Paulo S. L. M. Barreto 
 vincent.rijmen@esat.kuleuven.ac.be Vincent Rijmen.

It was specifically designed one sleepless night for fun 
and for sharing encrypted messages with simplicity
between peoples on facebook who are not familiar
with gnupg or other RSA (or AES/Serpent/Two Fish in this case) 
implementations.

This small program is designed on GNU/Linux but
it's QT4 compilant so you can compile it in
all QT4 platforms, including Windows, BSD
and many others.

Compile it with QTCreator IDE
_________________________________________________
USAGE:

Meet your friend, by person or by phone (less secure
but if you are interested in real security probably you 
are well aware of RSA public key encryption :D 
or OTR and TOR for instant messaging).
Choose with your friend a secret passphrase that only
both of you will know.

Open QTJehuty

Enter the passphrase, decided with your friend, in the passphrase field.

Then Start to write your message in the clear text box,
Press "Encrypt" button and the text will be encrypted in a sequence like this

%'#!%/),--'.)&#+"./"+/-+&-#)"%)'$%%0$++(!%!)!/,-$(,(#(*)-0)0*&/#&.$'/')-,,,--%.''0+/(,0$#-"")(#.

This is a typical text encrypted with Jehuty. It's not a normal
Anubis output because it's translated in a human readable text
ready to be cut and paste on the facebook page of your friend
that will decrypt the message with jehuty and the same key
shared with you.

The key must be a char sequence of max 16 characters.

With the same key, when you receive a message encrypted by your friend,
paste it into the Cipher text box and click "Decrypt" button.
You'll see the decrypted text in the "Clear Text box".

Once more, the clear conversation is shown in the conversation text box at the
bottom.

Have fun!
