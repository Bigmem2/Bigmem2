
#include <Rcpp.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
using namespace Rcpp;

// [[Rcpp::export]]
int read_csv(std::string filepath) {

    std::ifstream rawInputFile(filepath);

    // CharacterVector x = CharacterVector::create( "foo", "bar" )  ;
    // NumericVector y   = NumericVector::create( 0.0, 1.0 ) ;
    // List z            = List::create( x, y ) ;

    std::string line;

    while(std::getline(rawInputFile, line)) {
      Rcout << line << std::endl;
    }

    return 0;
}


// [[Rcpp::export]]
void process_line(const char* start, const char* end) {
    std::string line(start, end);
    std::cout << line << std::endl;
}

// [[Rcpp::export]]
void process_csv_line(const char* start, const char* end) {
    std::string line(start, end);

    // vector to store the individual values
    std::vector<std::string> values;

    // use stringstream to parse the line
    std::stringstream ss(line);
    std::string item;

    while (std::getline(ss, item, ',')) {
        values.push_back(item);
    }

    // print the parsed values
    for(const auto& value : values) {
        Rcout << value << " ";
    }
    Rcout << std::endl;
}

// [[Rcpp::export]]
int read_csv_mmap(std::string filepath) {

    int fd = open(filepath.c_str(), O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("fstat");
        close(fd);
        return 1;
    }

    size_t length = sb.st_size;
    void* addr = mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    char* data = static_cast<char*>(addr);
    const char* start = data;
    const char* end = data + length;

    while (start < end) {
        const char* newline = static_cast<const char*>(memchr(start, '\n', end - start));
        if (newline) {
            process_csv_line(start, newline);
            start = newline + 1;
        } else {
            process_csv_line(start, end);
            break;
        }
    }

    if (munmap(addr, length) == -1) {
        perror("munmap");
    }

    close(fd);

    return 0;
}
