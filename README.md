# IPMT

CLI Application to search pattern on text files. It is part of the first college project of **IF767 - Processamento de Cadeias de Caracteres**, course of Computer Science of Federal University of Pernambuco. 



## Running the project

clone the repository

```bash
  git clone https://github.com/binidj/ipmt
```

Open the project folder

```bash
  cd ipmt
```

Compile the application

```bash
  sh compile.sh
```

The application binary **ipmt** will be at **bin/** folder

```bash
  cd bin/
```

Run it.

## Usage

#### Runs the application

```http
  ipmt [options] pattern textfile [textfile...]\n"
```

| Argument   | Type       | Description                           |
| :---------- | :--------- | :---------------------------------- |
| `pattern` | `string` | **Required** when not using *--pattern* option. Pattern to be searched. |
| `textfile` | `path` | **Required**. Text file path. More than one text file can be processed with multiple arguments or wildcards. |

## Extra options

| Command             | Function                                                |
| --------------------| ---------------------------------------------------------------- |
| `-e` `--edit emax` | Searches all aproximated ocurrences of the pattern with some distance *emax*
| `-p` `--pattern patternfile` | Specifies the *file* from which patterns will be read and used on the search
| `-a` `--algorithm algorithm_name` | Specifies the algorithm *algorithm_name* that will be used on the pattern search
| `-c` `--count` | Prints the number of ocurrences of the specified pattern on all text files.
| `-n` `--noacopt` | Disables optmization for large number of occurences when using *aho_corasick* algoritm.
| `-h` `--help` | Basic IPMT options


#### Algorithms:
| Name         | String             |
| -------------| -------------------|
| Sliding Window | `sliding_window`
| KMP | `kmp`
| Boyer Moore | `boyer_moore`
| Sellers | `sellers`
| Wu Mamber | `wu_mamber`
| Aho Corasick | `aho_corasick`

README.md
