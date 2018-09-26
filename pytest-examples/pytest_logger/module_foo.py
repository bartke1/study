import logging

logger = logging.getLogger('foo')

def foo():
    logger.debug('this is debug message from foo')
    logger.warning('this is warning from foo')
