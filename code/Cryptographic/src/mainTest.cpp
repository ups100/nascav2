#include "CryptographicFactory.h"
#include "SymetricAlgorithm.h"
#include "AsymetricAlgorithm.h"

#include <iostream>
#include <string>
#include <cstdio>
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace INZ_project::Cryptographic;

void sprintfTest()
{
    char *hexData = new char[3];
    char ch = 'A';
    unsigned char *a = (unsigned char *)&ch;
    sprintf(&(hexData[0]), "%02X", a[0]);

    delete[] hexData;
}
char *stringToHex(const char *str, int size)
{
    char *hexData = new char[2 * size + 2];
    sprintfTest();
    hexData[2 *size + 1] = '\0';
    for (int i = 0; i < size; ++i) {
        sprintf(&(hexData[2 * i]), "%02X", ((const unsigned char *)str)[i]);
    }
    return hexData;
}

int main(void)
{
    char *hexData;
    boost::shared_ptr<SymetricAlgorithm> sym = boost::shared_ptr<SymetricAlgorithm>(CryptographicFactory::getSymAlgorithm("AES_CBC"));
    boost::shared_ptr<AsymetricAlgorithm> asym = boost::shared_ptr<AsymetricAlgorithm> (CryptographicFactory::getAsymAlgorithm(
            "RSA_OAEP_SHA"));

    //////////////////////////////////////AES
    sym->generateKey(32, 16);

    cout << "AES key (HEX) : " << endl;

    hexData = stringToHex(sym->getKey().data(), sym->getKey().size());

    cout << hexData << endl;
    delete[] hexData;

    cout << "AES IV (HEX) : " << endl;

    hexData = stringToHex(sym->getIv().data(), sym->getIv().size());

    cout << hexData << endl;
    delete[] hexData;
    //////////////////////////////////////RSA
    asym->generateKeys(128);

    cout << "RSA public key (HEX) : " << endl;

    hexData = stringToHex(asym->getPublicKey().data(),
            asym->getPublicKey().size());

    cout << hexData << endl;
    delete[] hexData;

    cout << "RSA private key (HEX) : " << endl;

    hexData = stringToHex(asym->getPrivateKey().data(),
            asym->getPrivateKey().size());

    cout << hexData << endl;
    delete[] hexData;

    /////////////////////////////////////ENCRYPTION AES
    string plain;
    cout << "Enter Text for encryption: " << endl;
    cin>>plain;

    QByteArray encryptedArray = sym->encrypt(QByteArray(plain.c_str()));

    hexData = stringToHex(encryptedArray.data(), encryptedArray.size());
    cout << "Encrypted AES (HEX): " << endl;
    cout << hexData << endl;
    delete[] hexData;

    /////////////////////////////////////ENCRYPTION RSA
    QByteArray encryptedArrayAsym = asym->encrypt(QByteArray(plain.c_str()));

    hexData = stringToHex(encryptedArrayAsym.data(), encryptedArrayAsym.size());
    cout << "Encrypted RSA (HEX): " << endl;
    cout << hexData << endl;
    delete[] hexData;

    ////////////////////////////////////Decryption AES
    QByteArray decrypted = sym->decrypt(encryptedArray);
    cout << "Recovered AES : " << endl;
    cout << decrypted.data() << endl;

    ////////////////////////////////////Decryption RSA
    decrypted = asym->decrypt(encryptedArrayAsym);
    cout << "Recovered RSA : " << endl;
    cout << decrypted.data() << endl;
}
