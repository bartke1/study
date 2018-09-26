import logging

logger = logging.getLogger('bar')

def bar():
    logger.debug('this is debug message from bar')
    logger.warning('this is warning from bar')
