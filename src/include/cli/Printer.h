/**
 * @file        Printer.h
 * @brief       Các hàm tiện ích in thông báo ra console với định dạng chuẩn.
 *
 * Cung cấp các hàm print có prefix rõ ràng (INFO, ERROR, WARNING)
 * để dễ theo dõi output của chương trình.
 *
 * @author      QCodesDS
 * @date        2026-05-13
 * @version     1.0
 */

#ifndef PRINTER_H
#define PRINTER_H

#include <string>

/**
 * @brief In thông báo thông tin (INFO) ra stdout.
 * @param msg Nội dung thông báo
 */
void printInfo(const std::string &msg);

/**
 * @brief In thông báo lỗi (ERROR) ra stderr.
 * @param msg Nội dung thông báo lỗi
 */
void printError(const std::string &msg);

/**
 * @brief In thông báo cảnh báo (WARNING) ra stderr.
 * @param msg Nội dung thông báo cảnh báo
 */
void printWarning(const std::string &msg);

#endif // PRINTER_H