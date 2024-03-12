#include <stdio.h>    
#include <stdlib.h>    
#include <string.h>    
#include <ctype.h>    
    
#define MAX_LENGTH 10000 // 假设论文最大长度为10000字符    
    
// 函数声明    
void normalize(char *str);  // 规范化字符串函数声明  
float calculateOverlap(const char *original, const char *plagiarized);  // 计算重复率函数声明  
void writeAnswer(float overlap, const char *answerPath);  // 写入答案函数声明  
    
int main(int argc, char *argv[]) {    
    if (argc != 4) {    
        printf("Usage: %s <original_file_path> <plagiarized_file_path> <answer_file_path>\n", argv[0]);    
        return 1;    
    }    
    
    const char *originalPath = argv[1];  // 原文文件路径  
    const char *plagiarizedPath = argv[2];  // 抄袭版论文文件路径  
    const char *answerPath = argv[3];  // 答案文件路径  
    
    FILE *originalFile = fopen(originalPath, "r");  // 打开原文文件  
    if (originalFile == NULL) {    
        perror("Error opening original file");  // 打开文件失败，输出错误信息  
        return 1;    
    }    
    
    FILE *plagiarizedFile = fopen(plagiarizedPath, "r");  // 打开抄袭版论文文件  
    if (plagiarizedFile == NULL) {    
        perror("Error opening plagiarized file");  // 打开文件失败，输出错误信息  
        fclose(originalFile);  // 关闭已打开的文件  
        return 1;    
    }    
    
    char original[MAX_LENGTH] = {0};  // 原文内容  
    char plagiarized[MAX_LENGTH] = {0};  // 抄袭版论文内容  
    
    if (fgets(original, MAX_LENGTH, originalFile) == NULL) {    
        perror("Error reading original file");  // 读取原文文件失败，输出错误信息  
        fclose(originalFile);  // 关闭已打开的文件  
        fclose(plagiarizedFile);  // 关闭已打开的文件  
        return 1;    
    }    
    
    if (fgets(plagiarized, MAX_LENGTH, plagiarizedFile) == NULL) {    
        perror("Error reading plagiarized file");  // 读取抄袭版论文文件失败，输出错误信息  
        fclose(originalFile);  // 关闭已打开的文件  
        fclose(plagiarizedFile);  // 关闭已打开的文件  
        return 1;    
    }    
    
    fclose(originalFile);  // 关闭原文文件  
    fclose(plagiarizedFile);  // 关闭抄袭版论文文件  
    
    normalize(original);  // 规范化原文内容  
    normalize(plagiarized);  // 规范化抄袭版论文内容  
    
    float overlap = calculateOverlap(original, plagiarized);  // 计算重复率  
    writeAnswer(overlap, answerPath);  // 写入答案到答案文件  
    
    return 0;  // 程序正常退出  
}    
    
void normalize(char *str) {    
    for (int i = 0; str[i]; i++) {    
        str[i] = tolower(str[i]);  // 将字符转换为小写  
        if (!isalnum(str[i])) {  // 如果不是字母或数字  
            str[i] = ' ';  // 则替换为空格  
        }    
    }    
}    
float calculateOverlap(const char *original, const char *plagiarized) {  
    int originalLength = strlen(original);  
    int plagiarizedLength = strlen(plagiarized);  
    int overlapLength = 0;  
  
    for (int i = 0; i < plagiarizedLength; i++) {  
        for (int j = 0; j <= originalLength - plagiarizedLength + i; j++) {  
            int match = 1;  
            for (int k = 0; k < plagiarizedLength - i; k++) {  
                if (original[j + k] != plagiarized[i + k]) {  
                    match = 0;  
                    break;  
                }  
            }  
            if (match) {  
                overlapLength = plagiarizedLength - i;  
                break;  
            }  
        }  
    }  
  
    return (float)overlapLength / plagiarizedLength * 100.0;  
}  
void writeAnswer(float overlap, const char *answerPath) {  
    FILE *answerFile = fopen(answerPath, "w");  
    if (answerFile == NULL) {  
        perror("Error opening answer file");  
        exit(EXIT_FAILURE);  
    }  
  
    // 将重复率格式化为两位小数的字符串  
    char overlapStr[10];  
    snprintf(overlapStr, sizeof(overlapStr), "%.2f", overlap);  
  
    // 写入答案文件  
    fprintf(answerFile, "%s\n", overlapStr);  
  
    // 关闭文件  
    fclose(answerFile);  
}  
