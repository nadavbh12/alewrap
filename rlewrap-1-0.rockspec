package = 'rlewrap'
version = '1-0'

source = {
   url = 'git://github.com/nadavbh12/rlewrap.git'
}

description = {
  summary = "Rlewrap"
}

dependencies = { 'rle','image','paths'}
build = {
   type = "command",
   build_command = [[
cmake -E make_directory build;
cd build;
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="$(LUA_BINDIR)/.." -DCMAKE_INSTALL_PREFIX="$(PREFIX)";
$(MAKE)
   ]],
   install_command = "$(MAKE) -C build install"
}
