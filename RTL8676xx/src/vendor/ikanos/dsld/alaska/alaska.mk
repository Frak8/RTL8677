include $(BASE_DIR)/.config
ALASKA_APP_BUILD_DIR=$(BASE_DIR)/../fusiv_src/user/dsld/alaska
export CC_COMPILER     = $(TARGET_CC)

export VDSL_FLAGS = -DLINUX_2_6
$(ALASKA_APP_BUILD_DIR)/vdsld:
	(cd $(ALASKA_APP_BUILD_DIR); make) || exit;
	@cp $(ALASKA_APP_BUILD_DIR)/vdsld $(TARGET_DIR)/bin
	@cp $(ALASKA_APP_BUILD_DIR)/vdsl $(TARGET_DIR)/bin
	@cp $(ALASKA_APP_BUILD_DIR)/vdsl.conf $(TARGET_DIR)/etc


vdsld: $(ALASKA_APP_BUILD_DIR)/vdsld

vdsld-clean:

#############################################################
#
# Toplevel Makefile options
#
#############################################################
ifeq ($(strip $(CONFIG_FUSIV_USER_VDSLD)),y)
TARGETS+=vdsld
endif

