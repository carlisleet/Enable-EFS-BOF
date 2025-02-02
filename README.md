# Enable EFS BOF

Code that compiles either as an executble or a BOF that enables the Microsoft EFS service.

## Notes

To compile as an executable - gcc enable-efs.c -o enable-efs.exe
To compile as a bof -  gcc enable-efs.c -c -o enable-efs.x64.o -DCOMPILE_BOF

## Authors

carl0s