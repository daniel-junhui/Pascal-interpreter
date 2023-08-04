# use g++-10 or gcc-10
env = Environment(CXX = 'g++-10', CXXFLAGS = '-std=c++2a')
# for subdirectory usage, export env to subdirectory
# Export('env')

SConscript('calculator/SConscript', variant_dir='build/calculator')
SConscript('pascal/SConscript', variant_dir='build/pascal')