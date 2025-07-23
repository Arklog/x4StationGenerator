from pydantic import BaseModel


class ModuleModel(BaseModel):
    id: str
    name: str
