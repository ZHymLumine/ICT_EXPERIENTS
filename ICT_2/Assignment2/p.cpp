#include <iostream>
#include <cmath>
#include <string>
using namespace std;

int main()
{
    string symbols = "LZ77_and_LZ78_are_the_two_lossless_data_compression_algorithms_published_in_papers_by_Abraham_Lempel_and_Jacob_Ziv_in_1977_and_1978._They_are_also_known_as_LZ1_and_LZ2_respectively._These_two_algorithms_form_the_basis_for_many_variations_including_LZW,_LZSS,_LZMA_and_others._Besides_their_academic_influence,_these_algorithms_formed_the_basis_of_several_ubiquitous_compression_schemes,_including_GIF_and_the_DEFLATE_algorithm_used_in_PNG_and_ZIP.";
    string decoded = "LZ77_and_LZ78_are_the_two_lossless_data_compression_algorithms_published_in_papers_by_Abraham_Lempel_and_Jacob_Ziv_in_1977_and_1978._They_are_also_known_as_LZ1_and_LZ2_respectively._These_two_algorithms_form_the_basis_for_many_variations_including_LZW,_LZSS,_LZMA_and_others._Besides_their_academic_influence,_these_algorithms_formed_the_basis_of_several_ubiquitous_compression_schemes,_including_GIF_and_the_DEFLATE_algorithm_used_in_PNG_and_ZIP.";
    string encoded = "0100110000101101000001101111101011111000011000010000110111000001100100000010111110001010110100011001110001000011000010000011100100000011001011000011101000000011010001101010111110000001110100000000111011100000011011110100001101100100110111001100000011100110000001101100011010111001110110010111110011101100001100010110000101000011000111001101101101000000111000001100011001011011001110011000000011010010100110110111000101101101100000000011001110100110111001010000101110100001111011011010110010111000000000001110101000000011000100101110110100101011001101000001101011001000010000110100100011001011111011110011000010111100110010100110001110011001000011000100000000111100100100001000001101010011100100001010110100000010101101101001000010011000011010110110111000101101100001011011011100001110101111100000001001010000101011000110100110110001000010000101101001000010111011001011100110111000010000011000100000000011100100000110011011101111000110010010001000011100100010100010111000010000101010000011110110010101101000101111100001010111001000100000110000100101110111001100100110101111100000000110101100001100110111100100100110111000010110111001101110010101101000000000011000110001110101111100010010011001000010000111001000110000111000000011010110001100100010110100100000000111011000011010110110001101000010111010010100110100000110000110010100011100111011110100000110000100101110110011101001010110100100100010110100000000000110110100110010110011001001010110110100011100110100000100000110001000001010111001101000010111001100010000110011001001010101111111001110110000100001100111100100010000111011010011010110100100001010111010001000010110111100001100111001110000110110001100101110111010100001110110100100001100110011110101010101011100000000010110010101010101001100000000101001111111000101111100010010100110100000000010000010101011101101111011001100110010100110010001011100000100001000010000110000110100100000111011001010001100101110100010010110110100100001100010111110011111101100001100001110110110100100001011000110100001101100110011110000110010100000110011000110000110100101100011010100110010100010110011001010010001101100111011001010111010000100111011100110110111001101111000011000110110100101101010111111000001101011111001010100110000100010110011010010001100101101111000000000110011000001000011100110000110101110110000011010111001000000101011011000000100001110101001010100110100100000000011100010010100101101001000100010110111100101001011100110001110001101111011001110111000000011111011100111001110001101111001011110111001100000000011000110100101101101101000110000010110001110111011011000010100101100100001000010110111000100100010111110000000001000111000000000100100100000000010001100101011101110100010010110101111100000000010001000000000001000101101101110100110010000001010101001011101101011111101000100110011110010101011010000110011101011111101010000110010100111101011010010010111101010000000000000100111010110101010111110000010101101110001111010101101010110110010100000000000000101110";
    cout << symbols.size() * 8 << ' ' << encoded.size()<<endl;
    string a = "01100001001100010010110001010011000011000110001000101100001";
    string b = "01100001001100010010110001010011000011000110001000101100001";

    string aa = "00001000110001010101";
    string bb = "00001000110001010101";
    if(aa == bb) cout << "2-9 right" << endl;
    cout << b.size() << endl;
    if(a == b) cout << "yes" << endl;
    if(symbols == decoded) cout << "decode correct" << endl;
    cout << ceil(-log2(0.0064)) + 1<< endl;

    string ex1 = "000000000000001000100110101000101011000011101";
    string dec = "aadaadaabcccbdbcbacc";
    string eg = "aadaadaabcccbdbcbacc";
    if(dec == eg) cout << "2-9 right" << endl;
    return 0;
}