make
echo "Dir contents before creating database: "
ls -lt
./bin/program < input1
echo "Dir contents after creating database: "
ls -lt
echo "Running program with remaining tests: "
./bin/program < input2