from pydantic import BaseModel

class CLFModelParams(BaseModel):
    """Parameters that are required for Classifier ML model prediction.
    To be passed to main.py for prediction function.

    Args:
        BaseModel (class): A base class for creating Pydantic models.
    """
    sbytes: int        
    smean: int         
    dmean: int         
    dbytes: int        
    dload: float       
    sload: float        
    dpkts: int        
    rate: float        
    dttl: int        
    spkts: int         