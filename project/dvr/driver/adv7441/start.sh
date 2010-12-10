#!/bin/sh

MODULE_NAME="adv7441"
MODULE_DESC="adv7441"

source $HCM_CVSBASE_SCRIPTS/common.sh

module_start_notify()
{
	case $1 in
	"combine_source_check")
		[ -f $dir_combine_source/Makefile ] && notify_rcode=checkok
		;;
	"prebuild_check")
		notify_rcode=checkok
		;;	
	"build_check")
		[ `find . -name "*.ko" | wc -l` == 1 ] && notify_rcode=checkok
		;;

	"install_headers")

		;;
	"install_standee")
		$CP *.ko $dir_install_standee/. -f
		$CP *.ko $HCM_ROOTFS_EXT_KO/. -f
		;;

	"install_check")
		[ `find $dir_install_standee -name "*.ko" | wc -l` == 1 ] && \
			notify_rcode=checkok
		;;
				
	"postbuild_cptorelease")
		;;	
		
	esac
}

source $HCM_CVSBASE_SCRIPTS/module_start.sh

