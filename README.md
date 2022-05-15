# IPMT

CLI Application to search patterns on indexed files and compress/decompress files. It is part of the second college project of **IF767 - Processamento de Cadeias de Caracteres**, course of Computer Science of Federal University of Pernambuco. 



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

## Usages

```http
  ipmt index textfile\n");
```

| Argument   | Type       | Description                           |
| :---------- | :--------- | :---------------------------------- |
| `textfile` | `path` | **Required**. Text file path to be indexed. |

```http
  ipmt search [options] pattern indexfile.idx\n");
```

| Argument   | Type       | Description                           |
| :---------- | :--------- | :---------------------------------- |
| `pattern` | `string` | **Required** when not using *--pattern* option. Pattern to be searched. |
| `indexfile.idx` | `path` | **Required**. Previously indexed .idx file path. |

```http
  ipmt zip textfile\n");
```

| Argument   | Type       | Description                           |
| :---------- | :--------- | :---------------------------------- |
| `textfile` | `path` | **Required**. Text file path to be compressed. |

```http
  ipmt unzip textfile.myz\n");
```

| Argument   | Type       | Description                           |
| :---------- | :--------- | :---------------------------------- |
| `textfile.myz` | `path` | **Required**. Previously compressed .myz file path.. |

## Extra options

| Command             | Function                                                |
| --------------------| ---------------------------------------------------------------- |
| `-p` `--pattern patternfile` | Specifies the *file* from which patterns will be read and used on the search
| `-c` `--count` | Prints the number of ocurrences of the specified pattern on all text files.
| `-h` `--help` | Basic IPMT options


#### Algorithms:
| Mode         | Algorithm          |
| -------------| -------------------|
| index | `Suffix Array`
| search | `Suffix Array`
| zip | `Huffman`
| unzip | `Huffman`

README.md
