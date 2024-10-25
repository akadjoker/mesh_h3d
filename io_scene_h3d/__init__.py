bl_info = {
        "name": "H3D Mesh Importer/Exporter",
    "author": "Luis Santos",
    "version": (2, 5, 1),
    "blender": (4, 0, 0),
    "location": "File > Import-Export",
    "description": "Import H3D Mesh",
    "category": "Import-Export",
}

import bpy
import os 
if "bpy" in locals():
    import importlib
    if "h3d_import" in locals():
        importlib.reload(h3d_import)

    
from .h3d_import import import_h3d
from .h3d_export import export_h3d


def read_model(context, filepath, texture_path):
    import_h3d(filepath, texture_path)
    return {'FINISHED'}




from bpy_extras.io_utils import ImportHelper, ExportHelper
from bpy.types import Operator
from bpy.props import StringProperty


class ImportH3DOperator(Operator, ImportHelper):
    bl_idname = "import_mesh.h3d"
    bl_label = "Import H3D"
    filename_ext = ".h3d"
    

    filepath: StringProperty(subtype="FILE_PATH")
    
    # Editbox para o caminho das texturas
    texturepath: StringProperty(
        name="Texture Path",
        description="Path to texture folder",
        default="",
        subtype="DIR_PATH"
    )

    def execute(self, context):
        if not self.filepath:
            self.report({'ERROR'}, "No file specified")
            return {'CANCELLED'}

        
        texture_path = self.texturepath
        if not texture_path:
            texture_path = os.path.dirname(self.filepath)  # Usar o mesmo diretório do arquivo H3D se o campo de entrada estiver vazio


        import_h3d(self.filepath, texture_path)

        return {'FINISHED'}

    def draw(self, context):
        layout = self.layout
        layout.prop(self, "texturepath")  

    def invoke(self, context, event):
        context.window_manager.fileselect_add(self)
        return {'RUNNING_MODAL'}

class ExportH3DOperator(bpy.types.Operator, ExportHelper):
    bl_idname = "export_mesh.h3d"
    bl_label = "Export H3D"
    filename_ext = ".h3d"
    
    filepath: StringProperty(subtype="FILE_PATH")

    def execute(self, context):
        if not self.filepath:
            self.report({'ERROR'}, "No file specified")
            return {'CANCELLED'}

        export_h3d(self.filepath)
        return {'FINISHED'}

def menu_func_import(self, context):
    self.layout.operator(ImportH3DOperator.bl_idname, text="Import H3D (.h3d)")

def menu_func_export(self, context):
    self.layout.operator(ExportH3DOperator.bl_idname, text="Export H3D (.h3d)")



def register():
    bpy.utils.register_class(ImportH3DOperator)
    bpy.utils.register_class(ExportH3DOperator)
    bpy.types.TOPBAR_MT_file_import.append(menu_func_import)
    bpy.types.TOPBAR_MT_file_export.append(menu_func_export)

def unregister():
    bpy.utils.unregister_class(ImportH3DOperator)
    bpy.utils.unregister_class(ExportH3DOperator)
    bpy.types.TOPBAR_MT_file_import.remove(menu_func_import)
    bpy.types.TOPBAR_MT_file_export.remove(menu_func_export)


if __name__ == "__main__":
    register()
