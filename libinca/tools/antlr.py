import os
import re

antlr_res={};

def make_re(classtype):
        return re.compile('^class\\s+(\\S+)\\s+extends\\s+'+classtype,re.MULTILINE);

def append_re(res,classtype,fn):
        res[classtype]=[make_re(classtype),fn];

def lexer_append(target,classname):
        target_append(target,classname);
        target.append(classname+'TokenTypes'+'.hpp');
        target.append(classname+'TokenTypes'+'.txt');

def target_append(target,classname):
        target.append(classname+'.hpp');
        target.append(classname+'.cpp');

append_re(antlr_res,'Lexer',lexer_append);
append_re(antlr_res,'Parser',target_append);
append_re(antlr_res,'TreeParser',target_append);

def antlr_emitter(target,source,env):
        target=[];
        for src in source:
                contents = src.get_contents();
                for type_re in antlr_res:
                        found = antlr_res[type_re][0].findall(contents);
                        for classname in found:
                                antlr_res[type_re][1](target,classname);
        return (target,source);

def generate(env):
    import Scons.Builder
    antlr_builder = Builder(action="java antlr.Tool $SOURCE",src_suffix='.g',emitter=antlr_emitter)
    env.Append(BUILDERS={'antlr':b}); # NEED TO APPEND!
    env.Append(ENV={'CLASSPATH':os.environ['CLASSPATH']});
