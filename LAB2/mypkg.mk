MYPKG_VERSION = 1.0
MYPKG_SITE = $(TOPDIR)/../app_reaction_time
MYPKG_SITE_METHOD = local
MYPKG_DEPENDENCIES = c-periphery

define MYPKG_BUILD_CMDS
$(MAKE) $(TARGET_CONFIGURE_OPTS) reaction_time -C $(@D)
endef
define MYPGK_INSTALL_TARGET_CMDS
$(INSTALL) -D -m 0755 $(@D)/reaction_time $(TARGET_DIR)/usr/bin
endef

MYPKG_LICENSE = Proprietary
$(eval $(generic-package))
