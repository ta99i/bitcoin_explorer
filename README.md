# Bitcoin Explorer Client

This project is a C++ Bitcoin client that allows you to explore and interact with the Bitcoin blockchain by handling blocks, transactions, and other related functionalities. It is specifically designed to help you read the binary files collected by a Bitcoin node. With this project, you can process and extract detailed information from these files.

### Note:

By default, the program processes and reads the binary files but does not store the extracted data. If you want to save the processed information to a file or a database, you will need to modify the code to implement this functionality.

## Project Structure

- **Makefile**: The build script for compiling the project.
- **main.cpp**: The main entry point for running the application.
- **src**: Contains the core logic for handling blockchain components:
  - `block.cpp`: Defines the structure and operations for blocks in the blockchain.
  - `transaction.cpp`: Handles Bitcoin transactions and their associated operations.
  - `utilities.cpp`: Utility functions used across the project to support various tasks.
- **blocks**: Presumed to contain blockchain data used or generated by the client.

## Setup and Installation

### Prerequisites

Before building the project, ensure the following dependencies are installed:

- C++ compiler (e.g., GCC or Clang)
- GNU Make (for compiling using the provided Makefile)
- **OpenSSL development libraries**: Install using the following command:

```bash
sudo apt install libssl-dev
```

### Compilation

To build the project, simply run the following command:

```bash
make
```

This will compile all source files and produce the executable file.

### Running the Application

Once compiled, you can run the Bitcoin explorer by executing the following command:

```bash
./bitcoin_explorer
```

### File Structure

- **Blocks**: Blockchain data, the format should be `blk0000.dat`.
- **main.cpp**: The main program logic and entry point.
- **src/block.cpp**: Handles the creation and manipulation of Bitcoin blocks.
- **src/transaction.cpp**: Responsible for managing Bitcoin transactions.
- **src/utilities.cpp**: Provides utility functions for various tasks within the program.

### Features

- **Block Processing**: The client can process blocks from the blockchain and extract relevant information.
- **Transaction Handling**: Includes logic to parse, verify, and manage Bitcoin transactions.
- **Binary File Reading**: The project allows you to read the binary files collected by a Bitcoin node and extract relevant blockchain data.
- **Utility Functions**: Helpers for data manipulation and format handling.

### Future Work

- **Multithreading**: Implement multithreading to enhance the performance of file processing.
- **Command Line Interface (CLI)**: Add a CLI to enable easier interaction with the client and make the tool more user-friendly.

### Contributing

If you'd like to contribute to this project, feel free to submit a pull request or report issues.

### License

This project is licensed under the MIT License.
