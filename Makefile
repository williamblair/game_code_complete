.PHONY: clean All

All:
	@echo "----------Building project:[ 13_3dScenes - Debug ]----------"
	@cd "16_3dScenes" && "$(MAKE)" -f  "13_3dScenes.mk"
clean:
	@echo "----------Cleaning project:[ 13_3dScenes - Debug ]----------"
	@cd "16_3dScenes" && "$(MAKE)" -f  "13_3dScenes.mk" clean
