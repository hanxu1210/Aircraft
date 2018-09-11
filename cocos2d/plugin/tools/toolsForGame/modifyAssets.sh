SELECTED_PLUGINS=(${NEED_PUBLISH//:/ })
for plugin_name in ${SELECTED_PLUGINS[@]}
do
    SRC_RES_DIR=${TARGET_ROOT}/${plugin_name}/android/ForAssets
    if [ -d "${SRC_RES_DIR}" ]; then
        cp -rf "${SRC_RES_DIR}"/* $1
    fi
done

exit 0
