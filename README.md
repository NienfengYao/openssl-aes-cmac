# openssl-aes-cmac
AES-CMAC example with openssl library

# Test
    make
    ./signtool -h
    ./signtool -t (Do test case)
    ./signtool -k (Read key[128-bits] from file and show the content)
    ./signtool -i file -c (Read data from file and do AES-CMAC operation)
    ./signtool -i file -o file -s (Read data from file and generate the signature)
    ./signtool -i file -k file -c (Read data/key from file and do AES-CMAC operation)
    ./signtool -i file -k file -o file -s (Read data/key from file and generate the signature)
    hexdump -n 48 image_signed.bin (Check the content of header of the signed file)
    echo -n -e \\x2b\\x7e\\x15\\x16\\x28\\xae\\xd2\\xa6\\xab\\xf7\\x15\\x88\\x09\\xcf\\x4f\\x3c > key.bin(Write key to file)

# Reference
* [RFC 4493: The AES-CMAC Algorithm](https://www.rfc-editor.org/rfc/rfc4493)
* [PyCryptodome](https://pycryptodome.readthedocs.io/en/latest/src/hash/cmac.html)
* [Compute AES CMAC using OpenSSL/libcrypto](http://rubenlaguna.com/post/2015-02-05-compute-aes-cmac-using-openssl-slash-libcrypto/)
  * [ecerulm/mac_example.c](https://gist.github.com/ecerulm/90653daf2b808aea0837)
  * `gcc mac_example.c -lcrypto`
* [CryptoJS extension: AES-CMAC and AES-SIV in JavaScript in the Browser](https://artjomb.github.io/cryptojs-extension/)
  * 可用來驗証，與 mac_example.c 的結果相同
* [megrxu/AES-CMAC](https://github.com/megrxu/AES-CMAC)
  * 除了 AES-CMAC，還包含了對 message 的 encryption/decryptopn.
