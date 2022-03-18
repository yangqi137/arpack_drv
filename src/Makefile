ARPACK_DRV_DYLIB = libarpack_drv.so
include local.makefile
all: $(ARPACK_DRV_DYLIB)
$(ARPACK_DRV_DYLIB): basic_drv.o herm_drv.o fortinfo_error.o
	$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) $^
clean:
	rm -f $(ARPACK_DRV_DYLIB) *.o
install: $(ARPACK_DRV_DYLIB)
	install -d $(INSTALL_DIR)/include/arpack_drv/bindings/
	install -m 644 bindings/*.hpp bindings/*.h $(INSTALL_DIR)/include/arpack_drv/bindings/
	install -m 644 *.hpp $(INSTALL_DIR)/include/arpack_drv
	install -m 644 $(ARPACK_DRV_DYLIB) $(INSTALL_DIR)/lib/
dep:
	rm -f .depend
	make .depend
.depend:
	echo # Program dependencies > .depend
	$(CXX) $(CPPFLAGS) -MM *.cpp >> .depend
include .depend
