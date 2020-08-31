LIBDIR = lib8001
DEPFILES = term.c
DEPEND = Depend

all: $(DEPEND) term.ihx 

term.ihx: term.rel term.lk $(LIBDIR)/lib8001.a $(LIBDIR)/crt0.rel
	sdldz80 -nf term.lk
	$(LIBDIR)/mkadr.pl *.sym $(LIBDIR)/*.sym > term.adr
	$(LIBDIR)/total.pl term.map

#$(LIBDIR)/lib8001.a:
$(LIBDIR)/lib8001.a: lib8001/base.c lib8001/utils.s
	make -C $(LIBDIR)

$(DEPEND):
	rm -f $(DEPEND)
	for file in $(DEPFILES); do sdcc -I$(LIBDIR) -MM $$file >> $(DEPEND); done

clean:
	rm -f $(DEPEND)
	rm -f *.{adr,asm,ihx,lst,map,noi,rel,sym}
	make -C $(LIBDIR) clean

%.rel: %.c
	sdcc -c -mz80 -I$(LIBDIR) $<
%.rel: %.s
	sdasz80 -lsw -o $@ $<

-include $(DEPEND)
