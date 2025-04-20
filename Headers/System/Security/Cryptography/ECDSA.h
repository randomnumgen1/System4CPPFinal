#pragma once
/*warning: random not implemented*/
#include <random> // Standard library random functionality
#include <System/BigInteger.h>
#include <limits>  
#include <cstdint> 
#include "SHA256.h"
namespace System::Security::Cryptography::AsymmetricAlgorithm {
    enum CURVETYPES {
        SECP256R1,
        SECP256K1
    };
    class ECDSA {
    public:
        ECDSA() {
            curveParams.p = BigInteger("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F");
            curveParams.a = BigInteger(0);
            curveParams.b = BigInteger(7);
            curveParams.n = BigInteger("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141");
            curveParams.G.x = BigInteger("0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798");
            curveParams.G.y = BigInteger("0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8");
            curveParams.G.isInfinity = false;
        }
        struct ECPoint {
            BigInteger x;
            BigInteger y;
            bool isInfinity; // Indicates the identity point
        };
        struct ECParams {
            BigInteger p;       // Prime modulus
            BigInteger a;       // Curve coefficient 'a'
            BigInteger b;       // Curve coefficient 'b'
            BigInteger n;       // Order of the base point
            ECPoint G;       // Base point (generator point)
        } curveParams;
        struct ECDSASignature {
            BigInteger r; // The x-coordinate of the curve point R, reduced modulo n
            BigInteger s; // The modular inverse calculation of the signature
        };
        // Function to convert private key to public key
        void setCurve(CURVETYPES curve) {
            switch (curve) {
            case CURVETYPES::SECP256K1:
                curveParams.p = BigInteger("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F");
                curveParams.a = BigInteger(0);
                curveParams.b = BigInteger(7);
                curveParams.n = BigInteger("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141");
                curveParams.G.x = BigInteger("0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798");
                curveParams.G.y = BigInteger("0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8");
                curveParams.G.isInfinity = false;
                break;
            case CURVETYPES::SECP256R1:
                curveParams.p = BigInteger("0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff");
                curveParams.a = BigInteger("0xffffffff00000001000000000000000000000000fffffffffffffffffffffffc");
                curveParams.b = BigInteger("0x5ac635d8aa3a93e7b3ebbd55769886bc651d06b0cc53b0f63bce3c3e27d2604b");
                curveParams.n = BigInteger("0xffffffff00000000ffffffffffffffffbce6faada7179e84f3b9cac2fc632551");
                curveParams.G.x = BigInteger("0x6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296");
                curveParams.G.y = BigInteger("0x4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5");
                curveParams.G.isInfinity = false;
                break;
            }
        }


        // Function to convert private key to public key
        ECPoint privtopubkey(BigInteger privateKey ) {
            ECPoint basePoint(curveParams.G.x, curveParams.G.y);
            // Ensure the private key is within valid range: 1 <= privateKey < curveOrder
            if (privateKey <= 0 || privateKey >= curveParams.n) {
                throw std::invalid_argument("Private key must be in range [1, curveOrder - 1].");
            }
            return scalarMultiplication(privateKey, basePoint);
        }

        bool isValidPoint(const ECPoint& P, const BigInteger& a, const BigInteger& b, const BigInteger& p) {
            if (P.isInfinity) return true;
            BigInteger lhs = (P.y * P.y) % p;
            BigInteger rhs = (P.x * P.x * P.x + a * P.x + b) % p;
            return lhs == rhs;
        }

        // Point Addition
        ECPoint pointAdd(const ECPoint& P, const ECPoint& Q, const BigInteger& p) {
            if (P.isInfinity) return Q;
            if (Q.isInfinity) return P;
            if (P.x == Q.x && (P.y + Q.y) % p == 0) {
                return ECPoint(); // Point at infinity
            }

            BigInteger lambda;
            if (P.x == Q.x && P.y == Q.y) {
                // Doubling case
                lambda = ((3 * P.x * P.x) * BigInteger::modInverse(2 * P.y, p)) % p;
            }
            else {
                // General addition case
                lambda = ((Q.y - P.y) * BigInteger::modInverse(Q.x - P.x, p)) % p;
            }

            BigInteger xR = (lambda * lambda - P.x - Q.x) % p;
            BigInteger yR = (lambda * (P.x - xR) - P.y) % p;

            return ECPoint((xR + p) % p, (yR + p) % p); // Ensure positive mod
        }

        // Point Doubling
        ECPoint pointDouble(const ECPoint& P, const BigInteger& a, const BigInteger& p) {
            if (P.isInfinity || P.y == 0) return ECPoint(); // Point at infinity

            BigInteger lambda = ((3 * P.x * P.x + a) * BigInteger::modInverse(2 * P.y, p)) % p;

            BigInteger xR = (lambda * lambda - 2 * P.x) % p;
            BigInteger yR = (lambda * (P.x - xR) - P.y) % p;

            return ECPoint((xR + p) % p, (yR + p) % p); // Ensure positive mod
        }

        // Scalar Multiplication
        ECPoint scalarMultiplication(BigInteger scalar, const ECPoint& point) {
            ECPoint result; // Point at infinity
            ECPoint current = point;

            while (scalar > 0) {
                if (scalar % 2 == 1) {
                    result = pointAdd(result, current, curveParams.p);
                }
                current = pointDouble(current, curveParams.a, curveParams.p);
                scalar /= 2;
                std::cout << "loop" << scalar << std::endl;
            }

            return result;
        }
        ECDSASignature sign(const BigInteger& privateKey, const uint8_t* message, size_t messageLength) {
             // Hash the message
            uint8_t hash[32] = {};
           System::Security::Cryptography::HashAlgorithm::SHA256 sha256;
            sha256.update(message, messageLength);
            sha256.finalize(hash);
            std::string hexString = toHexString(hash, 32);
            BigInteger z = BigInteger(hexString);
            BigInteger k, r, s;

            do {
                // Generate random 'k' such that 1 <= k < curveOrder
                //k = BigInteger::randomInRange(1, curveParams.n - 1);
                k = BigInteger(4);
                // Calculate R = k * G
                ECPoint R = scalarMultiplication(k, curveParams.G);
                r = R.x % curveParams.n;

                // If r == 0, retry with a new k
                if (r == 0) continue;

                // Compute s = (z + r * privateKey) * k^-1 mod n
                BigInteger kInverse = BigInteger::modInverse(k,curveParams.n);
                s = (kInverse * (z + r * privateKey)) % curveParams.n;

                // If s == 0, retry with a new k
            } while (s == 0);

            return { r, s };
        }
        bool verify(const ECDSASignature& signature, const BigInteger& publicKeyX, const BigInteger& publicKeyY, const uint8_t* message, size_t messageLength) {
            // Hash the message
            uint8_t hash[32] = {};
            System::Security::Cryptography::HashAlgorithm::SHA256 sha256;
            sha256.update(message, messageLength);
            sha256.finalize(hash);
            std::string hexString = toHexString(hash, 32);
            BigInteger z = BigInteger(hexString);
            BigInteger r = signature.r;
            BigInteger s = signature.s;

            // Verify 0 < r < n and 0 < s < n
            if (r <= 0 || r >= curveParams.n || s <= 0 || s >= curveParams.n) {
                return false;
            }

            // Compute w = s^-1 mod n
            BigInteger w = BigInteger::modInverse(s,curveParams.n);

            // Compute u1 = z * w mod n, u2 = r * w mod n
            BigInteger u1 = (z * w) % curveParams.n;
            BigInteger u2 = (r * w) % curveParams.n;

            // Calculate u1 * G + u2 * PublicKey
            ECPoint P = scalarMultiplication(u1, curveParams.G);
            ECPoint Q = scalarMultiplication(u2, { publicKeyX, publicKeyY, false });
            ECPoint R = pointAdd(P, Q, curveParams.p);

            // Check if R is at infinity or R.x % n != r
            if (R.isInfinity || (R.x % curveParams.n) != r) {
                return false;
            }

            return true;
        }

        std::string toHexString(const uint8_t* hash, size_t length) {
            std::ostringstream oss;
            oss << "0x"; // Prepend "0x"

            for (size_t i = 0; i < length; ++i) {
                oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
            }

            return oss.str();
        }





    };
}
