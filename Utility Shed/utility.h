/*
 * utility.h
 *
 *  Created on: Aug 24, 2023
 *      Author: user
 */

#ifndef UTILITY_H_
#define UTILITY_H_

int isNotProcedureName(const char *str);
int firstAlphaRestAlphaNum(const char *str);
void printFilePacket(const FilePacket *filePacket);
int extractRegisterNumber(const char *str);
void charTo12BitString(char c, char *bitString);
void intTo12BitBinary(int value, char *bitString);
int startsWith(const char *str, const char *prefix);
char toBase64(const char *binaryStr);
int isNumber(const char *str);
FILE *createOutputFileWithExtension(FilePacket *filePack,const char *extension, const char *permission);

#endif /* UTILITY_H_ */
