mod_kiko.la: mod_kiko.slo
	$(SH_LINK) -rpath $(libexecdir) -module -avoid-version  mod_kiko.lo
DISTCLEAN_TARGETS = modules.mk
shared =  mod_kiko.la
