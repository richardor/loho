#!/bin/sh

MODULE_NAME="hal"
MODULE_DESC="hal"

source $HCM_CVSBASE_SCRIPTS/common.sh

module_start_notify()
{
	case $1 in	
	"combine_source_check")
		[ -f $dir_combine_source/Makefile ] && notify_rcode=checkok
		;;
	"prebuild_check")
		;;	
	"build_check")
		;;
	"install_headers")
		;;
	"install_libs")
		$CP *.so $HCM_ROOTFS_LIB/. -f
		;;	
	"install_standee")
		$CP *.so $dir_install_standee/. -f
		;;

	"install_check")
		[ `find $dir_install_standee -name "*.so" | wc -l` == 1 ] && \
			notify_rcode=checkok
		;;
				
	"postbuild_cptorelease")
		;;	
		
	esac
}

source $HCM_CVSBASE_SCRIPTS/module_start.sh

